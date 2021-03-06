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
#include "schedule.h"
#include <QCoreApplication>

const QStringList Schedule::FREQUENCY_UNITS=(QStringList()<<"YEAR"<<"MONTH"<<"WEEK"<<"DAY");
const QStringList Schedule::TR_FREQUENCY_UNITS=(QStringList()<<Schedule::tr("year")
                                                             <<Schedule::tr("month")
                                                             <<Schedule::tr("week")
                                                             <<Schedule::tr("day"));

const QString Schedule::KW_FROM_YEAR="starty";
const QString Schedule::KW_FROM_MONTH="startm";
const QString Schedule::KW_FROM_DAY="startd";
const QString Schedule::KW_UNTIL_YEAR="stopy";
const QString Schedule::KW_UNTIL_MONTH="stopm";
const QString Schedule::KW_UNTIL_DAY="stopd";
const QString Schedule::KW_ENDLESS="endless";
const QString Schedule::KW_FREQ_VALUE="freq_value";
const QString Schedule::KW_FREQ_UNIT="freq_unit";

Schedule::Schedule(const QDate &from,
                   const QDate &until,
                   bool endless,
                   int freq_value,
                   Schedule::FrequencyUnit freq_unit) :
    m_from(from),
    m_until(until),
    m_endless(endless),
    m_freq_value(freq_value),
    m_freq_unit(freq_unit)
{

}

QString Schedule::freq_unit2str(Schedule::FrequencyUnit freq_unit)
{
    return FREQUENCY_UNITS.at(freq_unit);
}

Schedule::FrequencyUnit Schedule::str2freq_unit(const QString &freq_unit)
{
    return FrequencyUnit(FREQUENCY_UNITS.indexOf(freq_unit));
}

QString Schedule::freq_unit2trstr(Schedule::FrequencyUnit freq_unit)
{
    return TR_FREQUENCY_UNITS.at(freq_unit);
}

Schedule::FrequencyUnit Schedule::trstr2freq_unit(const QString &freq_unit)
{
    return Schedule::FrequencyUnit(TR_FREQUENCY_UNITS.indexOf(freq_unit));
}


bool Schedule::valid() const
{
    if(!m_from.isValid()) {
        return false;
    }
    if((!m_endless)&&(!m_until.isValid())) {
        return false;
    }
    if(m_freq_value<=0) {
        return false;
    }
    return true;
}

bool Schedule::contains(const QDate &date) const
{
    return date>=m_from&&date<=m_until;
}

QList<QDate> Schedule::dates(int year, int month) const
{
    QDate cdate=m_from,
          until=(m_endless?QDate::currentDate():m_until);

    QList<QDate> dates;
    do {
        /* check if date validates month & year constraints */
        if(year==-1||cdate.year()==year) {
            if(month==-1||cdate.month()==month) {
                /* append a valid date */
                dates.append(cdate);
            }
        }
        /* compute next date */
        switch (m_freq_unit) {
            case DAY:
                cdate=cdate.addDays(m_freq_value);
                break;
            case WEEK:
                cdate=cdate.addDays(m_freq_value*7);
                break;
            case MONTH:
                cdate=cdate.addMonths(m_freq_value);
                break;
            case YEAR:
                cdate=cdate.addYears(m_freq_value);
                break;
        }
    } while(cdate<=until);
    return dates;
}

void Schedule::update(const QDate &from,
                      const QDate &until,
                      bool endless,
                      int freq_value,
                      Schedule::FrequencyUnit freq_unit)
{
    m_from=from;
    m_until=until;
    m_endless=endless;
    m_freq_value=freq_value;
    m_freq_unit=freq_unit;
}
