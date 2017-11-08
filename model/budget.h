#ifndef BUDGET_H
#define BUDGET_H

#include "picsoumodelobj.h"

class Budget : public PicsouModelObj
{
public:
    Budget();
    Budget(double amount, QString name, QString description);
    virtual ~Budget();

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;
private:
    double _amount;
    QString _name;
    QString _description;
};

#endif // BUDGET_H
