#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "paymentmethod.h"
#include "scheduledoperation.h"
#include "operation.h"

#include <QHash>

class Account : public PicsouModelObj
{
public:
    virtual ~Account();
    Account();
    Account(QString name, QString description);

    inline void add_payment_method(PaymentMethod pm) { _payment_methods.insert(pm.id(), pm); }
    bool remove_payment_method(QUuid id);

    inline void add_scheduled_operation(ScheduledOperation sop) { _scheduled_ops.insert(sop.id(), sop); }
    bool remove_scheduled_operation(QUuid id);

    inline void add_operation(Operation op) { _ops.insert(op.id(), op); }
    bool remove_operation(QUuid id);

    inline QString name() const { return _name; }
    inline QString description() const { return _description; }
    inline QList<PaymentMethod> payment_methods() const { return _payment_methods.values(); }
    inline QList<ScheduledOperation> scheduled_ops() const { return _scheduled_ops.values(); }
    inline QList<Operation> ops() const { return _ops.values(); }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QString _name;
    QString _description;
    QHash<QUuid, PaymentMethod> _payment_methods;
    QHash<QUuid, ScheduledOperation> _scheduled_ops;
    QHash<QUuid, Operation> _ops;
};

#endif // ACCOUNT_H
