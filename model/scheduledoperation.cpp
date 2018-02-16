#include "scheduledoperation.h"

#define KW_FREQ "freq"
#define KEYS (QStringList() << KW_FREQ)

ScheduledOperation::ScheduledOperation() :
    Operation()
{

}

ScheduledOperation::ScheduledOperation(Frequency freq, double amount,
                                       QDate date,
                                       QString recipient,
                                       QString description,
                                       const PaymentMethod *payment_method) :
    Operation(amount,
              date,
              recipient,
              description,
              payment_method),
    _freq(freq)
{

}

ScheduledOperation::~ScheduledOperation()
{

}

bool ScheduledOperation::read(const QJsonObject &json)
{
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _freq = Frequency(qRound(json[KW_FREQ].toDouble()));
    _valid = Operation::read(json);
end:
    return _valid;
}

bool ScheduledOperation::write(QJsonObject &json) const
{
    /**/
    json[KW_FREQ] = _freq;
    return Operation::write(json);
}
