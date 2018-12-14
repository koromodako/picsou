#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QDate>
#include <QStringList>

#define KW_SD_START_YEAR "starty"
#define KW_SD_START_MONTH "startm"
#define KW_SD_START_DAY "startd"
#define KW_SD_STOP_YEAR "stopy"
#define KW_SD_STOP_MONTH "stopm"
#define KW_SD_STOP_DAY "stopd"
#define KW_SD_ENDLESS "endless"
#define KW_SD_FREQ_VALUE "freq_value"
#define KW_SD_FREQ_UNIT "freq_unit"

class Schedule
{
public:
    enum FrequencyUnit {
        YEAR  = 0,
        MONTH = 1,
        WEEK  = 2,
        DAY   = 3
    };

    Schedule(const QDate &start = QDate(),
             const QDate &stop = QDate(),
             bool endless = false,
             int freq_value = 0,
             Schedule::FrequencyUnit freq_unit = Schedule::YEAR);

    static QString freq_unit2str(FrequencyUnit freq_unit);
    static FrequencyUnit str2freq_unit(const QString &freq_unit);
    static QStringList frequency_units();

    inline QDate start() const { return _start; }
    inline QDate stop() const { return _stop; }
    inline bool endless() const { return _endless; }
    inline int freq_value() const { return _freq_value; }
    inline FrequencyUnit freq_unit() const { return _freq_unit; }

    bool valid() const;
    bool contains(const QDate &date) const;

private:
    QDate _start;
    QDate _stop;
    bool _endless;
    int _freq_value;
    FrequencyUnit _freq_unit;
};

#endif // SCHEDULE_H
