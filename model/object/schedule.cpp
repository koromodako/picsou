#include "schedule.h"

static QStringList gfrequency_units=QStringList()
        << "YEAR"
        << "MONTH"
        << "WEEK"
        << "DAY";

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
    return gfrequency_units.at(freq_unit);
}

Schedule::FrequencyUnit Schedule::str2freq_unit(const QString &freq_unit)
{
    return FrequencyUnit(gfrequency_units.indexOf(freq_unit));
}

QStringList Schedule::frequency_units()
{
    return gfrequency_units;
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
