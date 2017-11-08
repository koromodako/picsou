#include "user.h"

#include <QJsonArray>

User::User() :
    PicsouModelObj(false)
{

}

User::User(QString name) :
    PicsouModelObj(true),
    _name(name)
{

}

User::~User()
{

}

bool User::remove_budget(QUuid id)
{
    switch (_budgets.remove(id)) {
        case 0: /* TRACE */ return false;
        case 1: return true;
        default: /* TRACE */ return false;
    }
}

bool User::remove_account(QUuid id)
{
    switch (_accounts.remove(id)) {
        case 0: /* TRACE */ return false;
        case 1: return true;
        default: /* TRACE */ return false;
    }
}

bool User::read(const QJsonObject &json)
{
    QJsonArray array;
    /**/
    if(!json.contains("name") ||
       !json.contains("budgets") ||
       !json.contains("accounts")) {
        /* TRACE */
        _valid = false;
        goto end;
    }
    _name = json["name"].toString();
    JSON_READ_LIST(json, "budgets", _budgets, Budget)
    JSON_READ_LIST(json, "accounts", _accounts, Account)
    /**/
    _valid = true;
end:
    return _valid;
}

bool User::write(QJsonObject &json) const
{
    bool ok;
    QJsonArray array;
    QJsonObject obj;
    /**/
    json["name"] = _name;
    JSON_WRITE_LIST(json, "budgets", _budgets.values());
    JSON_WRITE_LIST(json, "accounts", _accounts.values());
    /**/
    ok = true;
end:
    return ok;
}
