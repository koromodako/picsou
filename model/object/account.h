#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "paymentmethod.h"
#include "scheduledoperation.h"
#include "../operationcollection.h"

#include <QHash>

#define KW_ACT_NAME "name"
#define KW_ACT_NOTES "notes"
#define KW_ACT_PAYMENT_METHODS "payment_methods"
#define KW_ACT_SCHEDULED_OPS "scheduled_ops"
#define KW_ACT_OPS "ops"

class Account : public PicsouModelObj
{
    Q_OBJECT
public:
    virtual ~Account();
    Account(PicsouModelObj *parent);
    Account(const QString &name,
            const QString &notes,
            PicsouModelObj *parent);

    void update(const QString &name,
                const QString &notes);

    void add_payment_method(const QString &name);
    bool remove_payment_method(QUuid id);

    void add_scheduled_operation(ScheduledOperationPtr sop) { _scheduled_ops.insert(sop->id(), sop); }
    bool remove_scheduled_operation(QUuid id);

    void add_operation(Amount amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method);
    void add_operations(OperationCollection ops);
    bool remove_operation(QUuid id);

    PaymentMethodPtr find_payment_method(QUuid id);
    ScheduledOperationPtr find_scheduled_operation(QUuid id);
    OperationPtr find_operation(QUuid id);

    inline QString name() const { return _name; }
    inline QString notes() const { return _notes; }
    PaymentMethodPtrList payment_methods(bool sorted=false) const;
    QStringList payment_methods_str(bool sorted=false) const;
    inline ScheduledOperationPtrList scheduled_ops() const { return _scheduled_ops.values(); }

    QList<int> years(bool sorted=false) const;
    OperationCollection ops(bool sorted=false) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const Account &other);

private:
    QString _name;
    QString _notes;
    QHash<QUuid, PaymentMethodPtr> _payment_methods;
    QHash<QUuid, ScheduledOperationPtr> _scheduled_ops;
    QHash<QUuid, OperationPtr> _ops;
};

DECL_PICSOU_MOD_OBJ_PTR(Account, AccountPtr, AccountPtrList);

#endif // ACCOUNT_H
