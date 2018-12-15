#include "scheduledoperation.h"
#include "utils/macro.h"

const QString ScheduledOperation::KW_NAME="name";

ScheduledOperation::ScheduledOperation(PicsouModelObj *parent) :
    Operation(parent)
{

}

ScheduledOperation::ScheduledOperation(double amount,
                                       const QString &budget,
                                       const QString &recipient,
                                       const QString &description,
                                       const QString &payment_method, const QString &name,
                                       const Schedule &schedule,
                                       PicsouModelObj *parent) :
    Operation(amount,
              QDate(),
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
                      QDate(),
                      budget,
                      recipient,
                      description,
                      payment_method);
}

bool ScheduledOperation::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>");
    static const QStringList keys=(QStringList()<<ScheduledOperation::KW_NAME
                                                <<Schedule::KW_START_YEAR
                                                <<Schedule::KW_START_MONTH
                                                <<Schedule::KW_START_DAY
                                                <<Schedule::KW_STOP_YEAR
                                                <<Schedule::KW_STOP_MONTH
                                                <<Schedule::KW_STOP_DAY
                                                <<Schedule::KW_ENDLESS
                                                <<Schedule::KW_FREQ_VALUE
                                                <<Schedule::KW_FREQ_UNIT);
    JSON_CHECK_KEYS(keys);
    _name=json[KW_NAME].toString();
    int start_y=json[Schedule::KW_START_YEAR].toInt();
    int start_m=json[Schedule::KW_START_MONTH].toInt();
    int start_d=json[Schedule::KW_START_DAY].toInt();
    int stop_y=json[Schedule::KW_STOP_YEAR].toInt();
    int stop_m=json[Schedule::KW_STOP_MONTH].toInt();
    int stop_d=json[Schedule::KW_STOP_DAY].toInt();
    bool endless=json[Schedule::KW_ENDLESS].toBool();
    int freq_value=json[Schedule::KW_FREQ_VALUE].toInt();
    Schedule::FrequencyUnit freq_unit=Schedule::str2freq_unit(json[Schedule::KW_FREQ_UNIT].toString());
    _schedule=Schedule(QDate(start_y, start_m, start_d),
                       QDate(stop_y, stop_m, stop_d),
                       endless, freq_value, freq_unit);
    set_valid(Operation::read(json));
    LOG_BOOL_RETURN(valid());
}

bool ScheduledOperation::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_NAME]=_name;
    json[Schedule::KW_START_YEAR]=_schedule.start().year();
    json[Schedule::KW_START_MONTH]=_schedule.start().month();
    json[Schedule::KW_START_DAY]=_schedule.start().day();
    json[Schedule::KW_STOP_YEAR]=_schedule.stop().year();
    json[Schedule::KW_STOP_MONTH]=_schedule.stop().month();
    json[Schedule::KW_STOP_DAY]=_schedule.stop().day();
    json[Schedule::KW_ENDLESS]=_schedule.endless();
    json[Schedule::KW_FREQ_VALUE]=_schedule.freq_value();
    json[Schedule::KW_FREQ_UNIT]=Schedule::freq_unit2str(_schedule.freq_unit());
    LOG_BOOL_RETURN(Operation::write(json));
}
