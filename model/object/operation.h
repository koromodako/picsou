#ifndef OPERATION_H
#define OPERATION_H

#include "../picsoumodelobj.h"
#include "amount.h"

#include <QDate>

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

    static const QString KW_AMOUNT;
    static const QString KW_DAY;
    static const QString KW_MONTH;
    static const QString KW_YEAR;
    static const QString KW_BUDGET;
    static const QString KW_RECIPIENT;
    static const QString KW_DESCRIPTION;
    static const QString KW_PAYMENT_METHOD;

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
