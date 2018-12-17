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

#include "utils/semver.h"
#include "model/object/user.h"
#include "model/operationcollection.h"

class PicsouDB : public PicsouDBO
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

    inline QDate timestamp() const { return m_timestamp; }
    inline SemVer version() const { return m_version; }
    inline QString name() const { return m_name; }
    inline QString description() const { return m_description; }

    UserPtr find_user(QUuid id) const;
    UserPtrList users(bool sorted=false) const;

    AccountPtr find_account(QUuid id) const;

    OperationCollection ops(QUuid account_id,
                            int year=-1,
                            int month=-1) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

private:
    QDate m_timestamp;
    SemVer m_version;
    QString m_name;
    QString m_description;
    QHash<QUuid, UserPtr> m_users;
};

DECL_PICSOU_MOD_OBJ_PTR(PicsouDB,
                        PicsouDBPtr,
                        PicsouDBShPtr,
                        PicsouDBPtrList,
                        PicsouDBShPtrList);

#endif // PICSOUDB_H
