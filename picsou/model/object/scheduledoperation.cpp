/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "scheduledoperation.h"
#include "utils/macro.h"

const QString ScheduledOperation::KW_NAME="name";

ScheduledOperation::ScheduledOperation(PicsouDBO *parent) :
    Operation(parent)
{

}

ScheduledOperation::ScheduledOperation(const Amount &amount,
                                       const QString &budget,
                                       const QString &srcdst,
                                       const QString &description,
                                       const QString &payment_method,
                                       const QString &name,
                                       const Schedule &schedule,
                                       PicsouDBO *parent) :
    Operation(false,
              amount,
              QDate(),
              budget,
              srcdst,
              description,
              payment_method,
              parent),
    m_name(name),
    m_schedule(schedule)
{

}

void ScheduledOperation::update(const Amount &amount,
                                const QString &budget,
                                const QString &srcdst,
                                const QString &description,
                                const QString &payment_method,
                                const QString &name,
                                const Schedule &schedule)
{
    m_name=name;
    m_schedule=schedule;
    Operation::update(false,
                      amount,
                      QDate(),
                      budget,
                      srcdst,
                      description,
                      payment_method);
}

bool ScheduledOperation::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>")
    static const QStringList keys=(QStringList()<<KW_NAME
                                                <<Schedule::KW_FROM_YEAR
                                                <<Schedule::KW_FROM_MONTH
                                                <<Schedule::KW_FROM_DAY
                                                <<Schedule::KW_UNTIL_YEAR
                                                <<Schedule::KW_UNTIL_MONTH
                                                <<Schedule::KW_UNTIL_DAY
                                                <<Schedule::KW_ENDLESS
                                                <<Schedule::KW_FREQ_VALUE
                                                <<Schedule::KW_FREQ_UNIT);
    JSON_CHECK_KEYS(keys);
    m_name=json[KW_NAME].toString();
    int start_y=json[Schedule::KW_FROM_YEAR].toInt();
    int start_m=json[Schedule::KW_FROM_MONTH].toInt();
    int start_d=json[Schedule::KW_FROM_DAY].toInt();
    int stop_y=json[Schedule::KW_UNTIL_YEAR].toInt();
    int stop_m=json[Schedule::KW_UNTIL_MONTH].toInt();
    int stop_d=json[Schedule::KW_UNTIL_DAY].toInt();
    bool endless=json[Schedule::KW_ENDLESS].toBool();
    int freq_value=json[Schedule::KW_FREQ_VALUE].toInt();
    Schedule::FrequencyUnit freq_unit=Schedule::str2freq_unit(json[Schedule::KW_FREQ_UNIT].toString());
    m_schedule=Schedule(QDate(start_y, start_m, start_d),
                       QDate(stop_y, stop_m, stop_d),
                       endless, freq_value, freq_unit);
    set_valid(Operation::read(json));
    LOG_BOOL_RETURN(valid())
}

bool ScheduledOperation::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_NAME]=m_name;
    json[Schedule::KW_FROM_YEAR]=m_schedule.from().year();
    json[Schedule::KW_FROM_MONTH]=m_schedule.from().month();
    json[Schedule::KW_FROM_DAY]=m_schedule.from().day();
    json[Schedule::KW_UNTIL_YEAR]=m_schedule.until().year();
    json[Schedule::KW_UNTIL_MONTH]=m_schedule.until().month();
    json[Schedule::KW_UNTIL_DAY]=m_schedule.until().day();
    json[Schedule::KW_ENDLESS]=m_schedule.endless();
    json[Schedule::KW_FREQ_VALUE]=m_schedule.freq_value();
    json[Schedule::KW_FREQ_UNIT]=Schedule::freq_unit2str(m_schedule.freq_unit());
    LOG_BOOL_RETURN(Operation::write(json))
}
