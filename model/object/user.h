#ifndef USER_H
#define USER_H

#include "budget.h"
#include "account.h"

#include <QHash>

#define KW_USR_NAME "name"
#define KW_USR_BUDGETS "budgets"
#define KW_USR_ACCOUNTS "accounts"

class User : public PicsouModelObj
{
    Q_OBJECT
public:
    virtual ~User();
    User(PicsouModelObj *parent);
    User(const QString &name,
         PicsouModelObj *parent);

    void update(const QString &name);

    void add_budget(double amount,
                    const QString &name,
                    const QString &description);
    bool remove_budget(QUuid id);

    void add_account(const QString &name,
                     const QString &description);
    bool remove_account(QUuid id);

    inline QString name() const { return _name; }
    BudgetPtrList budgets(bool sorted=false) const;
    QStringList budgets_str(bool sorted=false) const;
    AccountPtrList accounts(bool sorted=false) const;

    BudgetPtr find_budget(QUuid id) const;
    AccountPtr find_account(QUuid id) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const User &other);

private:
    QString _name;
    QHash<QUuid, BudgetPtr> _budgets;
    QHash<QUuid, AccountPtr> _accounts;
};

DECL_PICSOU_MOD_OBJ_PTR(User, UserPtr, UserPtrList);

#endif // USER_H
