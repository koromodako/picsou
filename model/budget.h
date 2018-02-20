#ifndef BUDGET_H
#define BUDGET_H

#include "picsoumodelobj.h"

class Budget : public PicsouModelObj
{
public:
    virtual ~Budget();
    Budget();
    Budget(double amount, QString name, QString description);

    inline double amount() const { return _amount; }
    inline QString name() const { return _name; }
    inline QString description() const { return _description; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const Budget &other);

private:
    double _amount;
    QString _name;
    QString _description;
};

#endif // BUDGET_H
