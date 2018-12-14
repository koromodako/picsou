#include "scheduledoperation.h"
#include "utils/macro.h"

#define KEYS (QStringList() << KW_SO_NAME \
                            << KW_SD_START_YEAR \
                            << KW_SD_START_MONTH \
                            << KW_SD_START_DAY \
                            << KW_SD_STOP_YEAR \
                            << KW_SD_STOP_MONTH \
                            << KW_SD_STOP_DAY \
                            << KW_SD_ENDLESS \
                            << KW_SD_FREQ_VALUE \
                            << KW_SD_FREQ_UNIT)

ScheduledOperation::ScheduledOperation(PicsouModelObj *parent) :
    Operation(parent)
{

}

ScheduledOperation::ScheduledOperation(double amount,
                                       const QDate &date,
                                       const QString &budget,
                                       const QString &recipient,
                                       const QString &description,
                                       const QString &payment_method, const QString &name,
                                       const Schedule &schedule,
                                       PicsouModelObj *parent) :
    Operation(amount,
              date,
              budget,
              recipient,
              description,
              payment_method,
              parent),
    _name(name),
    _schedule(schedule)
{

}

void ScheduledOperation::update(double amount,
                                const QDate &date,
                                const QString &budget,
                                const QString &recipient,
                                const QString &description,
                                const QString &payment_method,
                                const QString &name,
                                const Schedule &schedule)
{
    _name=name;
    _schedule=schedule;
    Operation::update(amount,
                      date,
                      budget,
                      recipient,
                      description,
                      payment_method);
}

ScheduledOperation::~ScheduledOperation()
{

}

bool ScheduledOperation::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>")
    JSON_CHECK_KEYS(KEYS);
    _name=json[KW_SO_NAME].toString();
    int start_y=json[KW_SD_START_YEAR].toInt();
    int start_m=json[KW_SD_START_MONTH].toInt();
    int start_d=json[KW_SD_START_DAY].toInt();
    int stop_y=json[KW_SD_STOP_YEAR].toInt();
    int stop_m=json[KW_SD_STOP_MONTH].toInt();
    int stop_d=json[KW_SD_STOP_DAY].toInt();
    bool endless=json[KW_SD_ENDLESS].toBool();
    int freq_value=json[KW_SD_FREQ_VALUE].toInt();
    Schedule::FrequencyUnit freq_unit=Schedule::str2freq_unit(json[KW_SD_FREQ_UNIT].toString());
    _schedule=Schedule(QDate(start_y, start_m, start_d),
                       QDate(stop_y, stop_m, stop_d),
                       endless, freq_value, freq_unit);
    set_valid(Operation::read(json));
    LOG_BOOL_RETURN(valid());
}

bool ScheduledOperation::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_SO_NAME]=_name;
    json[KW_SD_START_YEAR]=_schedule.start().year();
    json[KW_SD_START_MONTH]=_schedule.start().month();
    json[KW_SD_START_DAY]=_schedule.start().day();
    json[KW_SD_STOP_YEAR]=_schedule.stop().year();
    json[KW_SD_STOP_MONTH]=_schedule.stop().month();
    json[KW_SD_STOP_DAY]=_schedule.stop().day();
    json[KW_SD_ENDLESS]=_schedule.endless();
    json[KW_SD_FREQ_VALUE]=_schedule.freq_value();
    json[KW_SD_FREQ_UNIT]=Schedule::freq_unit2str(_schedule.freq_unit());
    LOG_BOOL_RETURN(Operation::write(json));
}
