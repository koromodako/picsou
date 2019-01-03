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
#include "operationcollection.h"
#include "utils/macro.h"

OperationCollection::OperationCollection(const Amount &initial_value) :
    m_initial_value(initial_value)
{
    clear();
}

OperationCollection::OperationCollection(const OperationShPtrList &ops, const Amount &initial_value) :
    m_initial_value(initial_value)
{
    clear();
    for(auto &op : ops) {
        append(op);
    }
}

void OperationCollection::clear()
{
    m_ops.clear();
    m_balance=0;
    m_total_debit=0;
    m_total_credit=0;
    m_expense_per_budget.clear();
}

void OperationCollection::append(const OperationShPtr &op)
{
    aggregate(op.data());
    m_ops.append(op);
}

bool op_cmp(const OperationShPtr &a, const OperationShPtr &b)
{
    return a->date()<b->date();
}

OperationShPtrList OperationCollection::list(bool sorted) const
{
    OperationShPtrList ops=m_ops;
    if(sorted) {
        std::sort(ops.begin(), ops.end(), op_cmp);
    }
    return ops;
}

void OperationCollection::aggregate(const Operation *op)
{
    Amount amount=op->amount();
    /* balance, total credit and total debit */
    m_balance+=amount;
    if(op->type()==Operation::DEBIT) {
        m_total_debit+=amount;
    } else {
        m_total_credit+=amount;
    }
    /* add op month to months set */
    m_years.insert(op->date().year());
    m_months.insert(op->date().month());
    QHash<QString, Amount>::iterator it;
    /* total expense per budget */
    it=m_expense_per_budget.find(op->budget());
    if(it!=m_expense_per_budget.end()) {
        it.value()+=amount;
    } else {
        m_expense_per_budget.insert(op->budget(), amount);
    }
    /* total expense per payment method */
    it=m_expense_per_pm.find(op->payment_method());
    if(it!=m_expense_per_pm.end()) {
        it.value()+=amount;
    } else {
        m_expense_per_pm.insert(op->payment_method(), amount);
    }
}

