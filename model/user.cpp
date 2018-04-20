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

User::User(PicsouModelObj *parent) :
    PicsouModelObj(false, parent)
{

}

User::User(const QString &name,
           PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
    _name(name)
{

}

void User::update(const QString &name)
{
    _name=name;
    emit modified();
}

void User::add_budget(double amount,
                      const QString &name,
                      const QString &description)
{
    BudgetPtr budget=BudgetPtr(new Budget(amount, name, description, this));
    _budgets.insert(budget->id(), budget);
    emit modified();
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

void User::add_account(const QString &name,
                       const QString &description)
{
    AccountPtr account=AccountPtr(new Account(name, description, this));
    _accounts.insert(account->id(), account);
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

bool budget_cmp(const BudgetPtr &a, const BudgetPtr &b)
{
    return a->name() < b->name();
}

BudgetPtrList User::budgets(bool sorted) const
{
    BudgetPtrList budgets=_budgets.values();
    if(sorted) {
        std::sort(budgets.begin(), budgets.end(), budget_cmp);
    }
    return budgets;
}

QStringList User::budgets_str(bool sorted) const
{
    QStringList budgets_str;
    BudgetPtrList budget_list=budgets(sorted);
    foreach (BudgetPtr budget, budget_list) {
        budgets_str << budget->name();
    }
    budgets_str.insert(0, tr("OTHER"));
    return budgets_str;
}

bool account_cmp(const AccountPtr &a, const AccountPtr &b)
{
    return a->name() < b->name();
}

AccountPtrList User::accounts(bool sorted) const
{
    AccountPtrList accounts=_accounts.values();
    if(sorted) {
        std::sort(accounts.begin(), accounts.end(), account_cmp);
    }
    return accounts;
}

BudgetPtr User::find_budget(QUuid id) const
{
    BudgetPtr budget;
    QHash<QUuid, BudgetPtr>::const_iterator it=_budgets.find(id);
    if(it!=_budgets.end()) {
        budget=*it;
    }
    return budget;
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
    _name=json[KW_NAME].toString();
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
    json[KW_NAME]=_name;
    JSON_WRITE_LIST(json, KW_BUDGETS, _budgets.values());
    JSON_WRITE_LIST(json, KW_ACCOUNTS, _accounts.values());
    /**/
    ok=true;
end:
    return ok;
}


bool User::operator <(const User &other)
{
    return (_name < other._name);
}
