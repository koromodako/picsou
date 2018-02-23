#ifndef OPERATION_H
#define OPERATION_H

#include "picsoumodelobj.h"

#include <QDate>

class PaymentMethod;
DECL_PICSOU_MOD_OBJ_PTR(PaymentMethod, PaymentMethodPtr);

class Operation : public PicsouModelObj
{
public:
    enum Type {
        CREDIT,
        DEBIT
    };

    virtual ~Operation();
    Operation(PicsouModelObj *parent);
    Operation(double amount,
              const QDate &date,
              const QString &budget,
              const QString &recipient,
              const QString &description,
              const QString &payment_method,
              PicsouModelObj *parent);

    inline double amount() const { return _amount; }
    inline QString amount_str() const { return QString("%0").arg(_amount, 0, 'f', 2); }
    inline QDate date() const { return _date; }
    inline QString budget() const { return _budget; }
    inline QString recipient() const { return _recipient; }
    inline QString description() const { return _description; }
    inline QString payment_method() const { return _payment_method; }
    inline Type type() const { return (_amount < 0. ? DEBIT : CREDIT); }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const Operation &other);

private:
    double _amount;
    QDate _date;
    QString _budget;
    QString _recipient;
    QString _description;
    QString _payment_method;
};

DECL_PICSOU_MOD_OBJ_PTR(Operation, OperationPtr);

#endif // OPERATION_H
