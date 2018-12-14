#ifndef SCHEDULEDOPERATION_H
#define SCHEDULEDOPERATION_H

#include "schedule.h"
#include "operation.h"

#define KW_SO_NAME "name"

class ScheduledOperation : public Operation
{
    Q_OBJECT
public:
    virtual ~ScheduledOperation();
    ScheduledOperation(PicsouModelObj *parent);
    ScheduledOperation(double amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method,
                       const QString &name,
                       const Schedule &schedule,
                       PicsouModelObj *parent);

    inline QString name() const { return _name; }
    inline Schedule schedule() const { return _schedule; }

    void update(double amount,
                const QDate &date,
                const QString &budget,
                const QString &recipient,
                const QString &description,
                const QString &payment_method,
                const QString &name,
                const Schedule &schedule);

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QString _name;
    Schedule _schedule;
};

DECL_PICSOU_MOD_OBJ_PTR(ScheduledOperation, ScheduledOperationPtr, ScheduledOperationPtrList);

#endif // SCHEDULEDOPERATION_H
