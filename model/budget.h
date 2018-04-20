#ifndef BUDGET_H
#define BUDGET_H

#include "picsoumodelobj.h"

class Budget : public PicsouModelObj
{
    Q_OBJECT
public:
    virtual ~Budget();
    Budget(PicsouModelObj *parent);
    Budget(double amount,
           const QString &name,
           const QString &description,
           PicsouModelObj *parent);

    void update(double amount,
                const QString &name,
                const QString &description);

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

DECL_PICSOU_MOD_OBJ_PTR(Budget, BudgetPtr, BudgetPtrList);

#endif // BUDGET_H
