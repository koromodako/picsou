#include "picsou.h"
#include "picsoudb.h"
#include "utils/macro.h"

#include <QDebug>

#define KEYS (QStringList() << KW_DB_NAME  \
                            << KW_DB_DESCRIPTION \
                            << KW_DB_USERS)

PicsouDB::~PicsouDB()
{
    DELETE_HASH_CONTENT(UserPtr, _users);
}

PicsouDB::PicsouDB() :
    PicsouModelObj(false, nullptr)
{

}

PicsouDB::PicsouDB(SemVer version,
                   const QString &name,
                   const QString &description) :
    PicsouModelObj(true, nullptr),
    _name(name),
    _version(version),
    _description(description)
{

}

void PicsouDB::add_user(const QString &username)
{
    UserPtr user=UserPtr(new User(username, this));
    _users.insert(user->id(), user);
    emit modified();
}

bool PicsouDB::remove_user(QUuid id)
{
    bool success=false;
    switch (_users.remove(id)) {
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
    UserPtrList users=_users.values();
    if(sorted) {
        std::sort(users.begin(), users.end(), user_cmp);
    }
    return users;
}

UserPtr PicsouDB::find_user(QUuid id) const
{
    UserPtr user;
    QHash<QUuid, UserPtr>::const_iterator it=_users.find(id);
    if(it!=_users.end()) {
        user=*it;
    }
    return user;
}

OperationCollection PicsouDB::ops(QUuid account_id,
                                  int year,
                                  int month,
                                  bool sorted) const
{
    OperationCollection selected_ops;
    AccountPtr account=find_account(account_id);
    foreach (OperationPtr op, account->ops(sorted).list()) {
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
    foreach (UserPtr user, _users.values()) {
        account=user->find_account(id);
        if(account->valid()) {
            break;
        }
    }
    return account;
}

bool PicsouDB::read(const QJsonObject &json)
{
    if(!json.contains(KW_DB_VERSION)) {
        LOG_CRITICAL("database file does not contain database version.");
        set_valid(false);
        goto end;
    }
    /**/
    _version=SemVer(json[KW_DB_VERSION].toString());
    if(_version<PICSOU_DB_VERSION) {
        LOG_DEBUG("older version of the DB file format: " << _version.to_str());
        set_valid(false);
        goto end;
    }
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name=json[KW_DB_NAME].toString();
    _description=json[KW_DB_DESCRIPTION].toString();
    JSON_READ_LIST(json, KW_DB_USERS, _users, User, this);
    /**/
    set_valid();
end:
    return valid();
}

bool PicsouDB::write(QJsonObject &json) const
{
    bool ok;
    QJsonObject obj;
    QJsonArray array;
    /**/
    json[KW_DB_NAME]=_name;
    json[KW_DB_VERSION]=_version.to_str();
    json[KW_DB_DESCRIPTION]=_description;
    JSON_WRITE_LIST(json, KW_DB_USERS, _users.values());
    /**/
    ok=true;
end:
    return ok;
}
