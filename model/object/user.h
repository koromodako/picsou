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
#ifndef USER_H
#define USER_H

#include "budget.h"
#include "account.h"

#include <QHash>

class User : public PicsouDBO
{
    Q_OBJECT
public:
    static const QString KW_NAME;
    static const QString KW_BUDGETS;
    static const QString KW_ACCOUNTS;

    virtual ~User();
    User(PicsouDBO *parent);
    User(const QString &name,
         PicsouDBO *parent);

    void update(const QString &name);

    void add_budget(double amount,
                    const QString &name,
                    const QString &description);
    bool remove_budget(QUuid id);

    void add_account(const QString &name,
                     const QString &description);
    bool remove_account(QUuid id);

    inline QString name() const { return m_name; }
    BudgetPtrList budgets(bool sorted=false) const;
    QStringList budgets_str(bool sorted=false) const;
    AccountPtrList accounts(bool sorted=false) const;

    BudgetPtr find_budget(QUuid id) const;
    AccountPtr find_account(QUuid id) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const User &other);

private:
    QString m_name;
    QHash<QUuid, BudgetPtr> m_budgets;
    QHash<QUuid, AccountPtr> m_accounts;
};

DECL_PICSOU_MOD_OBJ_PTR(User,
                        UserPtr,
                        UserShPtr,
                        UserPtrList,
                        UserShPtrList);

#endif // USER_H
