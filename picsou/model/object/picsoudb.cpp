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
    UserShPtr user=UserShPtr(new User(username, pswd, this));
    m_users.insert(user->id(), user);
    emit modified();
}

bool PicsouDB::remove_user(QUuid id, QString &error)
{
    bool success=false;
    switch (m_users.remove(id)) {
    case 0:
        error=tr("Failed to remove user from database: absent user.");
        break;
    case 1:
        success=true;
        emit modified();
        break;
    default:
        error=tr("Failed to remove user from database: more than one user removed.");
        break;
    }
    return success;
}

bool user_cmp(const UserShPtr &a, const UserShPtr &b)
{
    return a->name()<b->name();
}

UserShPtrList PicsouDB::users(bool sorted) const
{
    UserShPtrList users=m_users.values();
    if(sorted) {
        std::sort(users.begin(), users.end(), user_cmp);
    }
    return users;
}


UserShPtr PicsouDB::find_user(QUuid id) const
{
    UserShPtr user;
    QHash<QUuid, UserShPtr>::const_iterator it=m_users.find(id);
    if(it!=m_users.end()) {
        user=*it;
    }
    return user;
}

UserShPtr PicsouDB::find_user(const QString &name) const
{
    for(const auto &user : m_users.values()) {
        if(user->name()==name) {
            return user;
        }
    }
    return UserShPtr();
}

OperationCollection PicsouDB::ops(QUuid account_id,
                                  int year,
                                  int month,
                                  const QDate &until) const
{
    AccountShPtr account=find_account(account_id);
    if(account.isNull()) {
        LOG_WARNING("failed to find account.");
        return OperationCollection();
    }
    OperationCollection selected_ops(account->initial_amount());
    for(const auto &sop : account->scheduled_ops()) {
        LOG_DEBUG("sop->name="<<sop->name());
        for(const auto &date : sop->schedule().dates(year, month)) {
            LOG_DEBUG("generated date="<<date);
            if(until.isValid()&&date>until) {
                break;
            }
            Operation *op=new Operation(true,
                                        sop->amount(),
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
        if(until.isValid()&&op->date()>until) {
            continue;
        }
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

AccountShPtr PicsouDB::find_account(QUuid id) const
{
    AccountShPtr account;
    for(const auto &user : m_users.values()) {
        account=user->find_account(id);
        if(account.isNull()) {
            continue;
        }
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
