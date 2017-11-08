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

    ScheduledOperation();
    ScheduledOperation(Frequency freq,
                       double amount,
                       QDate date,
                       QString description,
                       QString payment_method);
    virtual ~ScheduledOperation();

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;
private:
    Frequency _freq;
};

#endif // SCHEDULEDOPERATION_H
