#ifndef PAYMENTMETHOD_H
#define PAYMENTMETHOD_H

#include "picsoumodelobj.h"

class PaymentMethod : public PicsouModelObj
{
public:
    virtual ~PaymentMethod();
    PaymentMethod();
    PaymentMethod(QString name);

    inline QString name() const { return _name; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;
private:
    QString _name;
};

#endif // PAYMENTMETHOD_H
