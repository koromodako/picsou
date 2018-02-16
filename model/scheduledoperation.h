#ifndef SCHEDULEDOPERATION_H
#define SCHEDULEDOPERATION_H

#include "operation.h"

class PaymentMethod;

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
    ScheduledOperation();
    ScheduledOperation(Frequency freq,
                       double amount,
                       QDate date,
                       QString recipient,
                       QString description,
                       const PaymentMethod *payment_method);

    inline Frequency frequency() const { return _freq; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    Frequency _freq;
};

#endif // SCHEDULEDOPERATION_H
