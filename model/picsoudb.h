#ifndef PICSOUDB_H
#define PICSOUDB_H

#include "user.h"

class PicsouDB : public PicsouModelObj
{
public:
    virtual ~PicsouDB();
    PicsouDB();
    PicsouDB(QString fpath,
             uint version_major,
             uint version_minor,
             QString description);

    inline void add_user(User user) { _users.insert(user.id(), user); }
    bool remove_user(QUuid id);

    inline QString fpath() const { return _fpath; }
    inline QPair<uint, uint> version() const { return _version; }
    inline QString description() const { return _description; }
    inline QList<User> users() const { return _users.values(); }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QString _fpath;
    QPair<uint, uint> _version;
    QString _description;
    QHash<QUuid, User> _users;
};

#endif // PICSOUDB_H
