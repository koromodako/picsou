#ifndef PICSOUDB_H
#define PICSOUDB_H

#include "object/user.h"
#include "converter/semver.h"
#include "operationcollection.h"

class PicsouDB : public PicsouModelObj
{
    Q_OBJECT
public:
    static const QString KW_NAME;
    static const QString KW_USERS;
    static const QString KW_VERSION;
    static const QString KW_TIMESTAMP;
    static const QString KW_DESCRIPTION;

    virtual ~PicsouDB();
    PicsouDB();
    PicsouDB(SemVer version,
             const QString &name,
             const QString &description);

    void add_user(const QString &username);
    bool remove_user(QUuid id);

    inline QDate timestamp() const { return _timestamp; }
    inline SemVer version() const { return _version; }
    inline QString name() const { return _name; }
    inline QString description() const { return _description; }

    UserPtr find_user(QUuid id) const;
    UserPtrList users(bool sorted=false) const;

    AccountPtr find_account(QUuid id) const;

    OperationCollection ops(QUuid account_id,
                            int year=-1,
                            int month=-1,
                            bool sorted=true) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QDate _timestamp;
    SemVer _version;
    QString _name;
    QString _description;
    QHash<QUuid, UserPtr> _users;
};

DECL_PICSOU_MOD_OBJ_PTR(PicsouDB, PicsouDBPtr, PicsouDBPtrList);

#endif // PICSOUDB_H
