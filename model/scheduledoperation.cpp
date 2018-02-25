#include "scheduledoperation.h"

#define KW_FREQ "freq"
#define KEYS (QStringList() << KW_FREQ)

static QStringList gfrequencies=QStringList()
        << "YEARLY"
        << "QUARTERLY"
        << "MONTHLY"
        << "WEEKLY"
        << "DAILY";

ScheduledOperation::ScheduledOperation(PicsouModelObj *parent) :
    Operation(parent)
{

}

ScheduledOperation::ScheduledOperation(Frequency freq,
                                       double amount,
                                       const QDate &date,
                                       const QString &budget,
                                       const QString &recipient,
                                       const QString &description,
                                       const QString &payment_method,
                                       PicsouModelObj *parent) :
    Operation(amount,
              date,
              budget,
              recipient,
              description,
              payment_method,
              parent),
    _freq(freq)
{

}

QString ScheduledOperation::freq2str(Frequency freq)
{
    return gfrequencies.at(freq);
}

ScheduledOperation::Frequency ScheduledOperation::str2freq(const QString &freq)
{
    return Frequency(gfrequencies.indexOf(freq));
}

QStringList ScheduledOperation::frequencies()
{
    return gfrequencies;
}

ScheduledOperation::~ScheduledOperation()
{

}

bool ScheduledOperation::read(const QJsonObject &json)
{
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _freq=Frequency(qRound(json[KW_FREQ].toDouble()));
    set_valid(Operation::read(json));
end:
    return valid();
}

bool ScheduledOperation::write(QJsonObject &json) const
{
    /**/
    json[KW_FREQ]=_freq;
    return Operation::write(json);
}
