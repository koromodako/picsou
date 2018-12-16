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
    _timestamp(),
    _version(version),
    _name(name),
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
    foreach(OperationPtr op, account->ops(sorted).list()) {
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
    foreach(UserPtr user, _users.values()) {
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
        _timestamp=QDate::fromString(json[KW_TIMESTAMP].toString(), Qt::ISODate);
        if(_timestamp>QDate::currentDate()) {
            LOG_CRITICAL("database file seems to have been saved in the future.");
            set_valid(false);
            LOG_BOOL_RETURN(false);
        }
    }
    /**/
    _version=SemVer(json[KW_VERSION].toString());
    if(_version<PICSOU_DB_VERSION) {
        LOG_DEBUG("older version of the DB file format: "<<_version.to_str());
        set_valid(false);
        LOG_BOOL_RETURN(false);
    }
    /**/
    static const QStringList keys=(QStringList()<<PicsouDB::KW_NAME
                                                <<PicsouDB::KW_USERS
                                                <<PicsouDB::KW_VERSION
                                                <<PicsouDB::KW_DESCRIPTION);
    JSON_CHECK_KEYS(keys);
    /**/
    _name=json[KW_NAME].toString();
    _description=json[KW_DESCRIPTION].toString();
    JSON_READ_LIST(json, KW_USERS, _users, User, this);
    /**/
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool PicsouDB::write(QJsonObject &json) const
{
    QJsonObject obj;
    QJsonArray array;
    /**/
    json[KW_NAME]=_name;
    json[KW_VERSION]=_version.to_str();
    json[KW_DESCRIPTION]=_description;
    json[KW_TIMESTAMP]=QDate::currentDate().toString(Qt::ISODate);
    JSON_WRITE_LIST(json, KW_USERS, _users.values());
    /**/
    LOG_BOOL_RETURN(true);
}
