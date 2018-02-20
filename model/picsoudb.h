#ifndef PICSOUDB_H
#define PICSOUDB_H

#include "user.h"

class PicsouDB : public PicsouModelObj
{
public:
    virtual ~PicsouDB();
    PicsouDB();
    PicsouDB(uint version_major,
             uint version_minor,
             QString name,
             QString description);

    inline void add_user(User user) { _users.insert(user.id(), user); }
    bool remove_user(QUuid id);

    inline QPair<uint, uint> version() const { return _version; }
    inline QString name() const { return _name; }
    inline QString description() const { return _description; }
    QList<User> users(bool sorted=false) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QPair<uint, uint> _version;
    QString _name;
    QString _description;
    QHash<QUuid, User> _users;
};

#endif // PICSOUDB_H
