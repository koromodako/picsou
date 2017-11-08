#include "scheduledoperation.h"

ScheduledOperation::ScheduledOperation() :
    Operation()
{

}

ScheduledOperation::ScheduledOperation(Frequency freq, double amount,
                                       QDate date,
                                       QString description,
                                       QString payment_method) :
    Operation(amount, date, description, payment_method),
    _freq(freq)
{

}

ScheduledOperation::~ScheduledOperation()
{

}

bool ScheduledOperation::read(const QJsonObject &json)
{
    /**/
    if(!json.contains("freq")) {
        _valid = false;
        goto end;
    }
    _freq = Frequency(qRound(json["freq"].toDouble()));
    _valid = Operation::read(json);
end:
    return _valid;
}

bool ScheduledOperation::write(QJsonObject &json) const
{
    /**/
    json["freq"] = _freq;
    return Operation::write(json);
}
