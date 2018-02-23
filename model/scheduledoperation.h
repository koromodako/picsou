#ifndef SCHEDULEDOPERATION_H
#define SCHEDULEDOPERATION_H

#include "operation.h"

class ScheduledOperation : public Operation
{
public:
    enum Frequency {
           YEARLY = 0x01,
        QUARTERLY = 0x02,
          MONTHLY = 0x03,
           WEEKLY = 0x04,
            DAILY = 0x05
    };

    virtual ~ScheduledOperation();
    ScheduledOperation(PicsouModelObj *parent);
    ScheduledOperation(Frequency freq,
                       double amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method,
                       PicsouModelObj *parent);

    inline Frequency frequency() const { return _freq; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    Frequency _freq;
};

DECL_PICSOU_MOD_OBJ_PTR(ScheduledOperation, ScheduledOperationPtr);

#endif // SCHEDULEDOPERATION_H
