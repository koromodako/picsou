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
#ifndef OPERATIONCOLLECTION_H
#define OPERATIONCOLLECTION_H

#include <QList>
#include <QHash>
#include <QString>

#include "object/operation.h"

class OperationCollection
{
public:
    OperationCollection(const Amount &initial_value=0.);
    OperationCollection(const OperationShPtrList &ops, const Amount &initial_value=0.);

    void clear();
    void append(const OperationShPtr &op);

    inline int length() const { return list(false).length(); }
    inline int months() const { return m_months.size(); }
    inline Amount balance() const { return m_balance+m_initial_value; }
    inline Amount total_debit() const { return m_total_debit; }
    inline Amount total_credit() const { return m_total_credit; }
    inline QHash<QString, Amount> expense_per_budget() const { return m_expense_per_budget; }

    Amount expense_per_ym(int year, int month=0);
    Amount total_in_range(const QDate &from, const QDate &to);
    OperationShPtrList list(bool sorted=true) const;

protected:
    void aggregate(const Operation *op);

private:
    /* aggregation members */
    Amount m_initial_value;
    Amount m_total_debit;
    Amount m_total_credit;
    Amount m_balance;
    QSet<int> m_months;
    QHash<QString, Amount> m_expense_per_budget;
    QHash<int, QHash<int, Amount>> m_expense_per_ym;
    /* pointer storage members */
    OperationShPtrList m_ops;

};

#endif // OPERATIONCOLLECTION_H
