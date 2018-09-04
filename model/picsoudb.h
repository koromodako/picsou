#ifndef PICSOUDB_H
#define PICSOUDB_H

#include "object/user.h"
#include "converter/semver.h"
#include "operationcollection.h"

#define KW_DB_NAME "name"
#define KW_DB_VERSION "version"
#define KW_DB_DESCRIPTION "description"
#define KW_DB_USERS "users"

class PicsouDB : public PicsouModelObj
{
    Q_OBJECT
public:
    virtual ~PicsouDB();
    PicsouDB();
    PicsouDB(SemVer version,
             const QString &name,
             const QString &description);

    void add_user(const QString &username);
    bool remove_user(QUuid id);

    inline QString name() const { return _name; }
    inline SemVer version() const { return _version; }
    inline QString description() const { return _description; }
    UserPtrList users(bool sorted=false) const;

    UserPtr find_user(QUuid id) const;
    AccountPtr find_account(QUuid id) const;
    OperationCollection ops(QUuid account_id,
                            int year=-1,
                            int month=-1,
                            bool sorted=true) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QString _name;
    SemVer _version;
    QString _description;
    QHash<QUuid, UserPtr> _users;
};

DECL_PICSOU_MOD_OBJ_PTR(PicsouDB, PicsouDBPtr, PicsouDBPtrList);

#endif // PICSOUDB_H
