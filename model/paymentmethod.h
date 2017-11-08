#ifndef PAYMENTMETHOD_H
#define PAYMENTMETHOD_H

#include "picsoumodelobj.h"

class PaymentMethod : public PicsouModelObj
{
public:
    PaymentMethod();
    PaymentMethod(QString name);
    virtual ~PaymentMethod();

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;
private:
    QString _name;
};

#endif // PAYMENTMETHOD_H
