#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QDate>
#include <QStringList>

class Schedule
{
public:
    enum FrequencyUnit {
        YEAR,
        MONTH,
        WEEK,
        DAY
    };

    static const QString KW_FROM_YEAR;
    static const QString KW_FROM_MONTH;
    static const QString KW_FROM_DAY;
    static const QString KW_UNTIL_YEAR;
    static const QString KW_UNTIL_MONTH;
    static const QString KW_UNTIL_DAY;
    static const QString KW_ENDLESS;
    static const QString KW_FREQ_VALUE;
    static const QString KW_FREQ_UNIT;

    Schedule(const QDate &from = QDate(),
             const QDate &until = QDate(),
             bool endless = true,
             int freq_value = 0,
             FrequencyUnit freq_unit = YEAR);

    static QString freq_unit2str(FrequencyUnit freq_unit);
    static QString freq_unit2trstr(FrequencyUnit freq_unit);
    static FrequencyUnit str2freq_unit(const QString &freq_unit);
    static QStringList frequency_units();

    inline QDate from() const { return _from; }
    inline QDate until() const { return _until; }
    inline bool endless() const { return _endless; }
    inline int freq_value() const { return _freq_value; }
    inline FrequencyUnit freq_unit() const { return _freq_unit; }

    bool valid() const;
    bool contains(const QDate &date) const;

    void update(const QDate &from,
                const QDate &until,
                bool endless,
                int freq_value,
                FrequencyUnit freq_unit);

private:
    QDate _from;
    QDate _until;
    bool _endless;
    int _freq_value;
    FrequencyUnit _freq_unit;
};

#endif // SCHEDULE_H
