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
    Account(PicsouModelObj *parent);
    Account(const QString &name,
            const QString &description,
            PicsouModelObj *parent);

    void update(const QString &name,
                const QString &description);

    void add_payment_method(const QString &name);
    bool remove_payment_method(QUuid id);

    void add_scheduled_operation(ScheduledOperationPtr sop) { _scheduled_ops.insert(sop->id(), sop); }
    bool remove_scheduled_operation(QUuid id);

    void add_operation(double amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method);
    bool remove_operation(QUuid id);

    PaymentMethodPtr find_payment_method(QUuid id);
    ScheduledOperationPtr find_scheduled_operation(QUuid id);
    OperationPtr find_operation(QUuid id);

    inline QString name() const { return _name; }
    inline QString description() const { return _description; }
    QList<PaymentMethodPtr> payment_methods(bool sorted=false) const;
    QStringList payment_methods_str(bool sorted=false) const;
    inline QList<ScheduledOperationPtr> scheduled_ops() const { return _scheduled_ops.values(); }

    QList<int> years(bool sorted=false) const;
    QList<OperationPtr> ops(bool sorted=false) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const Account &other);

private:
    QString _name;
    QString _description;
    QHash<QUuid, PaymentMethodPtr> _payment_methods;
    QHash<QUuid, ScheduledOperationPtr> _scheduled_ops;
    QHash<QUuid, OperationPtr> _ops;
};

DECL_PICSOU_MOD_OBJ_PTR(Account, AccountPtr);

#endif // ACCOUNT_H
