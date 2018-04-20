#include "operationcollection.h"

OperationCollection::~OperationCollection()
{

}

OperationCollection::OperationCollection()
{
    clear();
}

OperationCollection::OperationCollection(const QList<OperationPtr> &ops)
{
    foreach (OperationPtr op, ops) {
        append(op);
    }
}

void OperationCollection::clear()
{
    foreach (OperationPtr op, _ops) {
        delete op;
    }
    _ops.clear();
    _balance = 0;
    _total_debit = 0;
    _total_credit = 0;
    _expense_per_budget.clear();
}

void OperationCollection::append(const OperationPtr &op)
{
    Amount amount;
    int year, month;
    QHash<int, Amount>::iterator mit;
    QHash<QString, Amount>::iterator it;
    QHash<int, QHash<int, Amount>>::iterator yit;

    _ops.append(op);

    amount=op->amount();

    if(op->type()==Operation::DEBIT) {
        _total_debit+=amount;
    } else {
        _total_credit+=amount;
    }

    _balance+=amount;

    it=_expense_per_budget.find(op->budget());
    if(it!=_expense_per_budget.end()) {
        it.value()+=amount;
    } else {
        _expense_per_budget.insert(op->budget(), amount);
    }

    year=op->date().year();
    month=op->date().month();
    yit=_expense_per_ym.find(year);
    if(yit!=_expense_per_ym.end()) {

        mit=yit.value().find(month);
        if(mit!=yit.value().end()) {
            mit.value()+=amount;
        } else {
            yit.value().insert(month, amount);
        }

    } else {
        yit=_expense_per_ym.insert(year, QHash<int, Amount>());
        yit.value().insert(month, amount);
    }
}

Amount OperationCollection::expense_per_ym(int year, int month)
{
    Amount amount=-1;
    QHash<int, Amount>::iterator mit;
    QHash<int, QHash<int, Amount>>::iterator yit;

    yit=_expense_per_ym.find(year);
    if(yit!=_expense_per_ym.end()) {
        if(month<1||month>12) {
            amount=0;
            foreach(Amount val, yit.value().values()) {
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

