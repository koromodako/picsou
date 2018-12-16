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
