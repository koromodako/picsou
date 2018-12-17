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
#include "user.h"
#include "utils/macro.h"


const QString User::KW_NAME="name";
const QString User::KW_BUDGETS="budgets";
const QString User::KW_ACCOUNTS="accounts";

User::~User()
{
    DELETE_HASH_CONTENT(BudgetPtr, m_budgets);
    DELETE_HASH_CONTENT(AccountPtr, m_accounts);
}

User::User(PicsouDBO *parent) :
    PicsouDBO(false, parent)
{

}

User::User(const QString &name,
           PicsouDBO *parent) :
    PicsouDBO(true, parent),
    m_name(name)
{

}

void User::update(const QString &name)
{
    m_name=name;
    emit modified();
}

void User::add_budget(double amount,
                      const QString &name,
                      const QString &description)
{
    BudgetPtr budget=BudgetPtr(new Budget(amount, name, description, this));
    m_budgets.insert(budget->id(), budget);
    emit modified();
}

bool User::remove_budget(QUuid id)
{
    bool success=false;
    switch (m_budgets.remove(id)) {
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
    m_accounts.insert(account->id(), account);
    emit modified();
}

bool User::remove_account(QUuid id)
{
    bool success=false;
    switch (m_accounts.remove(id)) {
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
    return a->name()<b->name();
}

BudgetPtrList User::budgets(bool sorted) const
{
    BudgetPtrList budgets=m_budgets.values();
    if(sorted) {
        std::sort(budgets.begin(), budgets.end(), budget_cmp);
    }
    return budgets;
}

QStringList User::budgets_str(bool sorted) const
{
    QStringList budgets_str;
    BudgetPtrList budget_list=budgets(sorted);
    for(auto &budget : budget_list) {
        budgets_str<<budget->name();
    }
    budgets_str.insert(0, tr("OTHER"));
    return budgets_str;
}

bool account_cmp(const AccountPtr &a, const AccountPtr &b)
{
    return a->name()<b->name();
}

AccountPtrList User::accounts(bool sorted) const
{
    AccountPtrList accounts=m_accounts.values();
    if(sorted) {
        std::sort(accounts.begin(), accounts.end(), account_cmp);
    }
    return accounts;
}

BudgetPtr User::find_budget(QUuid id) const
{
    BudgetPtr budget;
    QHash<QUuid, BudgetPtr>::const_iterator it=m_budgets.find(id);
    if(it!=m_budgets.end()) {
        budget=*it;
    }
    return budget;
}

AccountPtr User::find_account(QUuid id) const
{
    AccountPtr account;
    QHash<QUuid, AccountPtr>::const_iterator it=m_accounts.find(id);
    if(it!=m_accounts.end()) {
        account=*it;
    }
    return account;
}

bool User::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>");
    static const QStringList keys=(QStringList()<<User::KW_NAME
                                   <<User::KW_BUDGETS
                                   <<User::KW_ACCOUNTS);
    JSON_CHECK_KEYS(keys);
    m_name=json[KW_NAME].toString();
    JSON_READ_LIST(json, KW_BUDGETS, m_budgets, Budget, this);
    JSON_READ_LIST(json, KW_ACCOUNTS, m_accounts, Account, this);
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool User::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>");
    json[KW_NAME]=m_name;
    JSON_WRITE_LIST(json, KW_BUDGETS, m_budgets.values());
    JSON_WRITE_LIST(json, KW_ACCOUNTS, m_accounts.values());
    LOG_BOOL_RETURN(true);
}

bool User::operator <(const User &other)
{
    return (m_name<other.m_name);
}
