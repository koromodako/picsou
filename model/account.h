#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "paymentmethod.h"
#include "scheduledoperation.h"
#include "operation.h"

#include <QHash>

class Account : public PicsouModelObj
{
public:
    Account();
    virtual ~Account();

    inline void add_payment_method(PaymentMethod pm) { _payment_methods.insert(pm.id(), pm); }
    bool remove_payment_method(QUuid id);

    inline void add_scheduled_operation(ScheduledOperation sop) { _scheduled_ops.insert(sop.id(), sop); }
    bool remove_scheduled_operation(QUuid id);

    inline void add_operation(Operation op) { _ops.insert(op.id(), op); }
    bool remove_operation(QUuid id);

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;
private:
    QHash<QUuid, PaymentMethod> _payment_methods;
    QHash<QUuid, ScheduledOperation> _scheduled_ops;
    QHash<QUuid, Operation> _ops;
};

#endif // ACCOUNT_H
