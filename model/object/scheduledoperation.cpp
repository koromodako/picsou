#include "scheduledoperation.h"
#include "utils/macro.h"

#define KEYS (QStringList() << KW_SOP_FREQ)

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

void ScheduledOperation::update(Frequency freq,
                                double amount,
                                const QDate &date,
                                const QString &budget,
                                const QString &recipient,
                                const QString &description,
                                const QString &payment_method)
{
    _freq=freq;
    Operation::update(amount,
                      date,
                      budget,
                      recipient,
                      description,
                      payment_method);
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
    LOG_IN("<QJsonObject>")
    JSON_CHECK_KEYS(KEYS);
    _freq=Frequency(qRound(json[KW_SOP_FREQ].toDouble()));
    set_valid(Operation::read(json));
    LOG_BOOL_RETURN(valid());
}

bool ScheduledOperation::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_SOP_FREQ]=_freq;
    LOG_BOOL_RETURN(Operation::write(json));
}
