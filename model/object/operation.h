#ifndef OPERATION_H
#define OPERATION_H

#include "../picsoumodelobj.h"
#include "amount.h"

#include <QDate>

#define KW_OP_AMOUNT "amount"
#define KW_OP_DAY "day"
#define KW_OP_MONTH "month"
#define KW_OP_YEAR "year"
#define KW_OP_BUDGET "budget"
#define KW_OP_RECIPIENT "recipient"
#define KW_OP_DESCRIPTION "description"
#define KW_OP_PAYMENT_METHOD "paymentMethod"

class PaymentMethod;
DECL_PICSOU_MOD_OBJ_PTR(PaymentMethod, PaymentMethodPtr, PaymentMethodPtrList);

class Operation : public PicsouModelObj
{
    Q_OBJECT
public:
    enum Type {
        CREDIT,
        DEBIT
    };

    virtual ~Operation();
    Operation(PicsouModelObj *parent);
    Operation(Amount amount,
              const QDate &date,
              const QString &budget,
              const QString &recipient,
              const QString &description,
              const QString &payment_method,
              PicsouModelObj *parent);

    void update(Amount amount,
                const QDate &date,
                const QString &budget,
                const QString &recipient,
                const QString &description,
                const QString &payment_method);

    inline Amount amount() const { return _amount; }
    inline QDate date() const { return _date; }
    inline QString budget() const { return _budget; }
    inline QString recipient() const { return _recipient; }
    inline QString description() const { return _description; }
    inline QString payment_method() const { return _payment_method; }
    inline Type type() const { return (_amount < 0. ? DEBIT : CREDIT); }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    inline bool operator<(const Operation &other) { return _date<other._date; }

private:
    Amount _amount;
    QDate _date;
    QString _budget;
    QString _recipient;
    QString _description;
    QString _payment_method;
};

DECL_PICSOU_MOD_OBJ_PTR(Operation, OperationPtr, OperationPtrList);

#endif // OPERATION_H
