#include "user.h"

#define KW_NAME "name"
#define KW_BUDGETS "budgets"
#define KW_ACCOUNTS "accounts"
#define KEYS (QStringList() << KW_NAME << KW_BUDGETS << KW_ACCOUNTS)

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

QList<Budget> User::budgets(bool sorted) const
{
    QList<Budget> budgets = _budgets.values();
    if(sorted) {
        std::sort(budgets.begin(), budgets.end());
    }
    return budgets;
}

QList<Account> User::accounts(bool sorted) const
{
    QList<Account> accounts = _accounts.values();
    if(sorted) {
        std::sort(accounts.begin(), accounts.end());
    }
    return accounts;
}

bool User::read(const QJsonObject &json)
{
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name = json[KW_NAME].toString();
    JSON_READ_LIST(json, KW_BUDGETS, _budgets, Budget);
    JSON_READ_LIST(json, KW_ACCOUNTS, _accounts, Account);
    /**/
    _valid = true;
end:
    return _valid;
}

bool User::write(QJsonObject &json) const
{
    bool ok;
    /**/
    json[KW_NAME] = _name;
    JSON_WRITE_LIST(json, KW_BUDGETS, _budgets.values());
    JSON_WRITE_LIST(json, KW_ACCOUNTS, _accounts.values());
    /**/
    ok = true;
end:
    return ok;
}

bool User::operator <(const User &other)
{
    return (_name < other._name);
}
