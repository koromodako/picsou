#include "schedule.h"

const QString Schedule::KW_START_YEAR="starty";
const QString Schedule::KW_START_MONTH="startm";
const QString Schedule::KW_START_DAY="startd";
const QString Schedule::KW_STOP_YEAR="stopy";
const QString Schedule::KW_STOP_MONTH="stopm";
const QString Schedule::KW_STOP_DAY="stopd";
const QString Schedule::KW_ENDLESS="endless";
const QString Schedule::KW_FREQ_VALUE="freq_value";
const QString Schedule::KW_FREQ_UNIT="freq_unit";

static const QStringList FREQUENCY_UNITS=(QStringList() << "YEAR"
                                                        << "MONTH"
                                                        << "WEEK"
                                                        << "DAY");

Schedule::Schedule(const QDate &start,
                   const QDate &stop,
                   bool endless,
                   int freq_value,
                   Schedule::FrequencyUnit freq_unit) :
    _start(start),
    _stop(stop),
    _endless(endless),
    _freq_value(freq_value),
    _freq_unit(freq_unit)
{

}

QString Schedule::freq_unit2str(FrequencyUnit freq_unit)
{
    return FREQUENCY_UNITS.at(freq_unit);
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
    if(!_start.isValid()) {
        return false;
    }
    if((!_endless)&&(!_stop.isValid())) {
        return false;
    }
    if(_freq_value<=0) {
        return false;
    }
    return true;
}

bool Schedule::contains(const QDate &date) const
{
    return date >= _start && date <= _stop;
}
