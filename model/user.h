#ifndef USER_H
#define USER_H

#include "budget.h"
#include "account.h"

#include <QHash>

class User : public PicsouModelObj
{
public:
    User(PicsouModelObj *parent);
    User(QString name,
         PicsouModelObj *parent);
    virtual ~User();

    void add_budget(BudgetPtr b);
    bool remove_budget(QUuid id);

    void add_account(AccountPtr a);
    bool remove_account(QUuid id);

    inline QString name() const { return _name; }
    QList<BudgetPtr> budgets(bool sorted=false) const;
    QList<AccountPtr> accounts(bool sorted=false) const;

    AccountPtr find_account(QUuid id) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const User &other);

private:
    QString _name;
    QHash<QUuid, BudgetPtr> _budgets;
    QHash<QUuid, AccountPtr> _accounts;
};

DECL_PICSOU_MOD_OBJ_PTR(User, UserPtr);

#endif // USER_H
