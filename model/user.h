#ifndef USER_H
#define USER_H

#include "budget.h"
#include "account.h"

#include <QHash>

class User : public PicsouModelObj
{
public:
    User();
    User(QString name);
    virtual ~User();

    inline void add_budget(Budget b) { _budgets.insert(b.id(), b); }
    bool remove_budget(QUuid id);

    inline void add_account(Account a) { _accounts.insert(a.id(), a); }
    bool remove_account(QUuid id);

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;
private:
    QString _name;
    QHash<QUuid, Budget> _budgets;
    QHash<QUuid, Account> _accounts;
};

#endif // USER_H
