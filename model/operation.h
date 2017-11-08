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

    Operation();
    Operation(double amount,
              QDate date,
              QString description,
              QString payment_method);
    virtual ~Operation();

    inline Type type() { return (_amount < 0. ? DEBIT : CREDIT); }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;
private:
    double _amount;
    QDate _date;
    QString _description;
    QString _payment_method;
};

#endif // OPERATION_H
