#ifndef SCHEDULEDOPERATION_H
#define SCHEDULEDOPERATION_H

#include "operation.h"

class ScheduledOperation : public Operation
{
public:
    enum Frequency {
           YEARLY=0,
        QUARTERLY=1,
          MONTHLY=2,
           WEEKLY=3,
            DAILY=4
    };

    static QString freq2str(Frequency freq);
    static Frequency str2freq(const QString &freq);
    static QStringList frequencies();

    virtual ~ScheduledOperation();
    ScheduledOperation(PicsouModelObj *parent);
    ScheduledOperation(Frequency freq,
                       double amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method,
                       PicsouModelObj *parent);

    void update(Frequency freq,
                double amount,
                const QDate &date,
                const QString &budget,
                const QString &recipient,
                const QString &description,
                const QString &payment_method);

    inline Frequency frequency() const { return _freq; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    Frequency _freq;
};

DECL_PICSOU_MOD_OBJ_PTR(ScheduledOperation, ScheduledOperationPtr);

#endif // SCHEDULEDOPERATION_H
