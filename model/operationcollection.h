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
    virtual ~OperationCollection();
    OperationCollection();
    OperationCollection(const OperationPtrList &ops);

    void clear();
    void append(const OperationPtr &op);
    void append(const OperationShPtr &sh_op);

    inline int length() const { return _ops.length(); }
    inline Amount balance() const { return _balance; }
    inline Amount total_debit() const { return _total_debit; }
    inline Amount total_credit() const { return _total_credit; }
    inline QHash<QString, Amount> expense_per_budget() const { return _expense_per_budget; }

    Amount expense_per_ym(int year, int month=0);
    Amount total_in_range(const QDate &from, const QDate &to);
    OperationPtrList list(bool sorted=true) const;

protected:
    void aggregate(const Operation *op);

private:
    /* aggregation members */
    Amount _total_debit;
    Amount _total_credit;
    Amount _balance;
    QHash<QString, Amount> _expense_per_budget;
    QHash<int, QHash<int, Amount>> _expense_per_ym;
    /* pointer storage members */
    OperationPtrList _ops;
    OperationShPtrList _sh_ops;

};

#endif // OPERATIONCOLLECTION_H
