#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QDate>
#include <QStringList>

class Schedule
{
public:
    enum FrequencyUnit {
        YEAR  = 0,
        MONTH = 1,
        WEEK  = 2,
        DAY   = 3
    };

    static const QString KW_START_YEAR;
    static const QString KW_START_MONTH;
    static const QString KW_START_DAY;
    static const QString KW_STOP_YEAR;
    static const QString KW_STOP_MONTH;
    static const QString KW_STOP_DAY;
    static const QString KW_ENDLESS;
    static const QString KW_FREQ_VALUE;
    static const QString KW_FREQ_UNIT;

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
