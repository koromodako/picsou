/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef PICSOUDB_H
#define PICSOUDB_H

#include "object/user.h"
#include "utils/semver.h"
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
                            int month=-1) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QDate _timestamp;
    SemVer _version;
    QString _name;
    QString _description;
    QHash<QUuid, UserPtr> _users;
};

DECL_PICSOU_MOD_OBJ_PTR(PicsouDB,
                        PicsouDBPtr,
                        PicsouDBShPtr,
                        PicsouDBPtrList,
                        PicsouDBShPtrList);

#endif // PICSOUDB_H
