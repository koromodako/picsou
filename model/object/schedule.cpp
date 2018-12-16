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
#include <QObject>

const QString Schedule::KW_FROM_YEAR="starty";
const QString Schedule::KW_FROM_MONTH="startm";
const QString Schedule::KW_FROM_DAY="startd";
const QString Schedule::KW_UNTIL_YEAR="stopy";
const QString Schedule::KW_UNTIL_MONTH="stopm";
const QString Schedule::KW_UNTIL_DAY="stopd";
const QString Schedule::KW_ENDLESS="endless";
const QString Schedule::KW_FREQ_VALUE="freq_value";
const QString Schedule::KW_FREQ_UNIT="freq_unit";

static const QStringList FREQUENCY_UNITS=(QStringList()<<"YEAR"
                                                       <<"MONTH"
                                                       <<"WEEK"
                                                       <<"DAY");

Schedule::Schedule(const QDate &from,
                   const QDate &until,
                   bool endless,
                   int freq_value,
                   Schedule::FrequencyUnit freq_unit) :
    _from(from),
    _until(until),
    _endless(endless),
    _freq_value(freq_value),
    _freq_unit(freq_unit)
{

}

QString Schedule::freq_unit2str(FrequencyUnit freq_unit)
{
    return FREQUENCY_UNITS.at(freq_unit);
}

QString Schedule::freq_unit2trstr(FrequencyUnit freq_unit)
{
    switch (freq_unit) {
        case YEAR:
            return QObject::tr("year");
        case MONTH:
            return QObject::tr("month");
        case WEEK:
            return QObject::tr("week");
        case DAY:
            return QObject::tr("day");
    }
    return QObject::tr("undefined");
}

Schedule::FrequencyUnit Schedule::str2freq_unit(const QString &freq_unit)
{
    return FrequencyUnit(FREQUENCY_UNITS.indexOf(freq_unit));
}

QStringList Schedule::frequency_units()
{
    return FREQUENCY_UNITS;
}

bool Schedule::valid() const
{
    if(!_from.isValid()) {
        return false;
    }
    if((!_endless)&&(!_until.isValid())) {
        return false;
    }
    if(_freq_value<=0) {
        return false;
    }
    return true;
}

bool Schedule::contains(const QDate &date) const
{
    return date>=_from&&date<=_until;
}

QList<QDate> Schedule::dates(int year, int month) const
{
    QDate cdate=_from,
          until=(_endless?QDate::currentDate():_until);

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
        switch (_freq_unit) {
            case DAY:
                cdate=cdate.addDays(_freq_value);
                break;
            case WEEK:
                cdate=cdate.addDays(_freq_value*7);
                break;
            case MONTH:
                cdate=cdate.addMonths(_freq_value);
                break;
            case YEAR:
                cdate=cdate.addYears(_freq_value);
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
    _from=from;
    _until=until;
    _endless=endless;
    _freq_value=freq_value;
    _freq_unit=freq_unit;
}
