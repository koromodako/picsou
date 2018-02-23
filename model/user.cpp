#include "user.h"

#define KW_NAME "name"
#define KW_BUDGETS "budgets"
#define KW_ACCOUNTS "accounts"
#define KEYS (QStringList() << KW_NAME << KW_BUDGETS << KW_ACCOUNTS)

User::~User()
{
    DELETE_HASH_CONTENT(BudgetPtr, _budgets);
    DELETE_HASH_CONTENT(AccountPtr, _accounts);
}

void User::add_budget(BudgetPtr b)
{
    _budgets.insert(b->id(), b);
    emit modified();
}

User::User(PicsouModelObj *parent) :
    PicsouModelObj(false, parent)
{

}

User::User(QString name, PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
    _name(name)
{

}

bool User::remove_budget(QUuid id)
{
    bool success=false;
    switch (_budgets.remove(id)) {
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

void User::add_account(AccountPtr a)
{
    _accounts.insert(a->id(), a);
    emit modified();
}

bool User::remove_account(QUuid id)
{
    bool success=false;
    switch (_accounts.remove(id)) {
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

QList<BudgetPtr> User::budgets(bool sorted) const
{
    QList<BudgetPtr> budgets = _budgets.values();
    if(sorted) {
        std::sort(budgets.begin(), budgets.end());
    }
    return budgets;
}

QList<AccountPtr> User::accounts(bool sorted) const
{
    QList<AccountPtr> accounts = _accounts.values();
    if(sorted) {
        std::sort(accounts.begin(), accounts.end());
    }
    return accounts;
}

AccountPtr User::find_account(QUuid id) const
{
    AccountPtr account;
    QHash<QUuid, AccountPtr>::const_iterator it=_accounts.find(id);
    if(it!=_accounts.end()) {
        account=*it;
    }
    return account;
}

bool User::read(const QJsonObject &json)
{
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name = json[KW_NAME].toString();
    JSON_READ_LIST(json, KW_BUDGETS, _budgets, Budget, this);
    JSON_READ_LIST(json, KW_ACCOUNTS, _accounts, Account, this);
    /**/
    set_valid();
end:
    return valid();
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
