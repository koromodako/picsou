#ifndef PAYMENTMETHOD_H
#define PAYMENTMETHOD_H

#include "picsoumodelobj.h"

class PaymentMethod : public PicsouModelObj
{
public:
    virtual ~PaymentMethod();
    PaymentMethod(PicsouModelObj *parent);
    PaymentMethod(QString name, PicsouModelObj *parent);

    inline QString name() const { return _name; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const PaymentMethod &other);

private:
    QString _name;
};

DECL_PICSOU_MOD_OBJ_PTR(PaymentMethod, PaymentMethodPtr);

#endif // PAYMENTMETHOD_H
