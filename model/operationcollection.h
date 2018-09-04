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

    inline int length() const { return _ops.length(); }
    inline Amount balance() const { return _balance; }
    inline Amount total_debit() const { return _total_debit; }
    inline Amount total_credit() const { return _total_credit; }
    inline OperationPtrList list() const { return _ops; }
    inline QHash<QString, Amount> expense_per_budget() const { return _expense_per_budget; }

    Amount expense_per_ym(int year, int month=0);
    Amount total_in_range(const QDate &from, const QDate &to);

private:
    Amount _total_debit;
    Amount _total_credit;
    Amount _balance;
    QHash<QString, Amount> _expense_per_budget;
    OperationPtrList _ops;
    QHash<int, QHash<int, Amount>> _expense_per_ym;

};

#endif // OPERATIONCOLLECTION_H
