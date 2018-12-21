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

OperationCollection::~OperationCollection()
{

}

OperationCollection::OperationCollection()
{
    clear();
}

OperationCollection::OperationCollection(const OperationPtrList &ops)
{
    for(auto &op : ops) {
        append(op);
    }
}

void OperationCollection::clear()
{
    for(auto &op : m_ops) {
        delete op;
    }
    m_ops.clear();
    m_balance=0;
    m_total_debit=0;
    m_total_credit=0;
    m_expense_per_budget.clear();
}

void OperationCollection::append(const OperationPtr &op)
{
    aggregate(op.data());
    m_ops.append(op);
}

void OperationCollection::append(const OperationShPtr &sh_op)
{
    aggregate(sh_op.data());
    m_sh_ops.append(sh_op);
}

Amount OperationCollection::expense_per_ym(int year, int month)
{
    Amount amount=-1;
    QHash<int, Amount>::iterator mit;
    QHash<int, QHash<int, Amount>>::iterator yit;
    yit=m_expense_per_ym.find(year);
    if(yit!=m_expense_per_ym.end()) {
        if(month<1||month>12) {
            amount=0;
            for(auto &val : yit.value().values()) {
                amount+=val;
            }
        } else {
            mit=yit.value().find(month);
            if(mit!=yit.value().end()) {
                amount=mit.value();
            }
        }
    }
    return amount;
}

Amount OperationCollection::total_in_range(const QDate &from, const QDate &to)
{
    Amount total=0;
    for(const auto &op : m_ops) {
        if(op->date()>=from&&op->date()<=to) {
            total+=op->amount();
        }
    }
    return total;
}

bool op_cmp(const OperationPtr &a, const OperationPtr &b)
{
    return a->date()<b->date();
}

OperationPtrList OperationCollection::list(bool sorted) const
{
    OperationPtrList ops=m_ops;
    for(const auto &sh_op : m_sh_ops) {
        ops.append(OperationPtr(sh_op.data()));
    }
    if(sorted) {
        std::sort(ops.begin(), ops.end(), op_cmp);
    }
    return ops;
}

void OperationCollection::aggregate(const Operation *op)
{
    Amount amount=op->amount();
    if(op->type()==Operation::DEBIT) {
        m_total_debit+=amount;
    } else {
        m_total_credit+=amount;
    }

    m_balance+=amount;

    QHash<QString, Amount>::iterator it=m_expense_per_budget.find(op->budget());
    if(it!=m_expense_per_budget.end()) {
        it.value()+=amount;
    } else {
        m_expense_per_budget.insert(op->budget(), amount);
    }

    int year=op->date().year(),
        month=op->date().month();

    QHash<int, QHash<int, Amount>>::iterator yit=m_expense_per_ym.find(year);
    if(yit!=m_expense_per_ym.end()) {
        QHash<int, Amount>::iterator mit=yit.value().find(month);
        if(mit!=yit.value().end()) {
            mit.value()+=amount;
        } else {
            yit.value().insert(month, amount);
        }
    } else {
        yit=m_expense_per_ym.insert(year, QHash<int, Amount>());
        yit.value().insert(month, amount);
    }
}
