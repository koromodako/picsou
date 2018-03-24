#include "picsoudb.h"

#define KW_NAME "name"
#define KW_VERSION "version"
#define KW_DESCRIPTION "description"
#define KW_USERS "users"
#define KEYS (QStringList() << KW_NAME << KW_VERSION << KW_DESCRIPTION << KW_USERS)

PicsouDB::~PicsouDB()
{
    DELETE_HASH_CONTENT(UserPtr, _users);
}

PicsouDB::PicsouDB() :
    PicsouModelObj(false, nullptr)
{

}

PicsouDB::PicsouDB(uint version_major,
                   uint version_minor,
                   const QString &name,
                   const QString &description) :
    PicsouModelObj(true, nullptr),
    _name(name),
    _version(QString("%0.%1").arg(version_major).arg(version_minor)),
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
    return a->name() < b->name();
}

QList<UserPtr> PicsouDB::users(bool sorted) const
{
    QList<UserPtr> users=_users.values();
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

QList<OperationPtr> PicsouDB::ops(QUuid account_id,
                                  int year,
                                  int month,
                                  bool sorted) const
{
    QList<OperationPtr> selected_ops;
    AccountPtr account=find_account(account_id);
    foreach (OperationPtr op, account->ops(sorted)) {
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
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name=json[KW_NAME].toString();
    _version=json[KW_VERSION].toString();
    _description=json[KW_DESCRIPTION].toString();
    JSON_READ_LIST(json, KW_USERS, _users, User, this);
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
    json[KW_NAME]=_name;
    json[KW_VERSION]=_version;
    json[KW_DESCRIPTION]=_description;
    JSON_WRITE_LIST(json, KW_USERS, _users.values());
    /**/
    ok=true;
end:
    return ok;
}
