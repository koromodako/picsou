#ifndef OPERATION_H
#define OPERATION_H

#include "picsoumodelobj.h"

#include <QDate>

class PaymentMethod;

class Operation : public PicsouModelObj
{
public:
    enum Type {
        CREDIT,
        DEBIT
    };

    virtual ~Operation();
    Operation();
    Operation(double amount,
              QDate date,
              QString recipient,
              QString description,
              const PaymentMethod *payment_method);

    inline double amount() const { return _amount; }
    inline QDate date() const { return _date; }
    inline QString recipient() const { return _recipient; }
    inline QString description() const { return _description; }
    inline QString payment_method() const { return _payment_method; }
    inline Type type() const { return (_amount < 0. ? DEBIT : CREDIT); }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    double _amount;
    QDate _date;
    QString _recipient;
    QString _description;
    QString _payment_method;
};

#endif // OPERATION_H
