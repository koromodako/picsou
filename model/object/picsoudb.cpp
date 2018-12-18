/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "picsou.h"
#include "picsoudb.h"
#include "utils/macro.h"

const QString PicsouDB::KW_NAME="name";
const QString PicsouDB::KW_USERS="users";
const QString PicsouDB::KW_VERSION="version";
const QString PicsouDB::KW_TIMESTAMP="timestamp";
const QString PicsouDB::KW_DESCRIPTION="description";

PicsouDB::~PicsouDB()
{
    DELETE_HASH_CONTENT(UserPtr, m_users);
}

PicsouDB::PicsouDB() :
    PicsouDBO(false, nullptr)
{

}

PicsouDB::PicsouDB(SemVer version,
                   const QString &name,
                   const QString &description) :
    PicsouDBO(true, nullptr),
    m_timestamp(),
    m_version(version),
    m_name(name),
    m_description(description)
{

}

void PicsouDB::add_user(const QString &username, const QString &pswd)
{
    UserPtr user=UserPtr(new User(username, pswd, this));
    m_users.insert(user->id(), user);
    emit modified();
}

bool PicsouDB::remove_user(QUuid id)
{
    bool success=false;
    switch (m_users.remove(id)) {
    case 0:
        /* TRACE */
        break;
    case 1:
        success=true;
        emit modified();
        break;
    default:
        /* TRACE */
        break;
    }
    return success;
}

bool user_cmp(const UserPtr &a, const UserPtr &b)
{
    return a->name()<b->name();
}

UserPtrList PicsouDB::users(bool sorted) const
{
    UserPtrList users=m_users.values();
    if(sorted) {
        std::sort(users.begin(), users.end(), user_cmp);
    }
    return users;
}

UserPtr PicsouDB::find_user(QUuid id) const
{
    UserPtr user;
    QHash<QUuid, UserPtr>::const_iterator it=m_users.find(id);
    if(it!=m_users.end()) {
        user=*it;
    }
    return user;
}

OperationCollection PicsouDB::ops(QUuid account_id,
                                  int year,
                                  int month) const
{
    OperationCollection selected_ops;
    AccountPtr account=find_account(account_id);
    for(const auto &sop : account->scheduled_ops()) {
        for(const auto &date : sop->schedule().dates(year, month)) {
            Operation *op=new Operation(sop->amount(),
                                        date,
                                        sop->budget(),
                                        sop->recipient(),
                                        sop->description(),
                                        sop->payment_method(),
                                        account.data());
            op->mark_scheduled();
            selected_ops.append(OperationShPtr(op));
        }
    }
    for(const auto &op : account->ops()) {
        if(year!=-1&&op->date().year()!=year) {
            continue;
        }
        if(month!=-1&&op->date().month()!=month) {
            continue;
        }
        selected_ops.append(op);
    }
    return selected_ops;
}

AccountPtr PicsouDB::find_account(QUuid id) const
{
    AccountPtr account;
    for(const auto &user : m_users.values()) {
        account=user->find_account(id);
        if(account->valid()) {
            break;
        }
    }
    return account;
}

bool PicsouDB::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>");
    if(!json.contains(KW_VERSION)) {
        LOG_CRITICAL("database file does not contain database version.");
        set_valid(false);
        LOG_BOOL_RETURN(false);
    }
    /**/
    if(json.contains(KW_TIMESTAMP)) {
        m_timestamp=QDate::fromString(json[KW_TIMESTAMP].toString(), Qt::ISODate);
        if(m_timestamp>QDate::currentDate()) {
            LOG_CRITICAL("database file seems to have been saved in the future.");
            set_valid(false);
            LOG_BOOL_RETURN(false);
        }
    }
    /**/
    m_version=SemVer(json[KW_VERSION].toString());
    if(m_version<PICSOU_DB_VERSION) {
        LOG_DEBUG("older version of the DB file format: "<<m_version.to_str());
        set_valid(false);
        LOG_BOOL_RETURN(false);
    }
    /**/
    static const QStringList keys=(QStringList()<<KW_NAME
                                                <<KW_USERS
                                                <<KW_VERSION
                                                <<KW_DESCRIPTION);
    JSON_CHECK_KEYS(keys);
    /**/
    m_name=json[KW_NAME].toString();
    m_description=json[KW_DESCRIPTION].toString();
    JSON_READ_LIST(json, KW_USERS, m_users, User, this);
    /**/
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool PicsouDB::write(QJsonObject &json) const
{
    QJsonObject obj;
    QJsonArray array;
    /**/
    json[KW_NAME]=m_name;
    json[KW_VERSION]=m_version.to_str();
    json[KW_DESCRIPTION]=m_description;
    json[KW_TIMESTAMP]=QDate::currentDate().toString(Qt::ISODate);
    JSON_WRITE_LIST(json, KW_USERS, m_users.values());
    /**/
    LOG_BOOL_RETURN(true);
}
