#ifndef PICSOUDB_H
#define PICSOUDB_H

#include "user.h"

class PicsouDB : public PicsouModelObj
{
public:
    PicsouDB();
    PicsouDB(QString fpath,
             uint version_major,
             uint version_minor,
             QString description);
    virtual ~PicsouDB();

    bool open();
    bool save();

    inline void add_user(User user) { _users.insert(user.id(), user); }
    bool remove_user(QUuid id);

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;
private:
    QString _fpath;
    QPair<uint, uint> _version;
    QString _description;
    QHash<QUuid, User> _users;
};

#endif // PICSOUDB_H
