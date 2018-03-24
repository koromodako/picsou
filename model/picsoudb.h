#ifndef PICSOUDB_H
#define PICSOUDB_H

#include "user.h"

class PicsouDB : public PicsouModelObj
{
    Q_OBJECT
public:
    virtual ~PicsouDB();
    PicsouDB();
    PicsouDB(uint version_major,
             uint version_minor,
             const QString &name,
             const QString &description);

    void add_user(const QString &username);
    bool remove_user(QUuid id);

    inline QString name() const { return _name; }
    inline QString version() const { return _version; }
    inline QString description() const { return _description; }
    QList<UserPtr> users(bool sorted=false) const;

    UserPtr find_user(QUuid id) const;
    AccountPtr find_account(QUuid id) const;
    QList<OperationPtr> ops(QUuid account_id,
                            int year=-1,
                            int month=-1) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QString _name;
    QString _version;
    QString _description;
    QHash<QUuid, UserPtr> _users;
};

DECL_PICSOU_MOD_OBJ_PTR(PicsouDB, PicsouDBPtr);

#endif // PICSOUDB_H
