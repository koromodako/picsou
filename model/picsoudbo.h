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
#ifndef PICSOUDBO_H
#define PICSOUDBO_H

#include <QUuid>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>

class PicsouDBO : public QObject
{
    Q_OBJECT
public:
    static const QString KW_WKEY;
    static const QString KW_DATA;

    PicsouDBO(bool valid, PicsouDBO *parent);

    inline QUuid id() const { return m_id; }
    inline bool valid() const { return m_valid; }
    inline bool wrapped() const { return m_wrapped; }

    inline void set_parent(PicsouDBO *parent) { m_parent=parent; }

    virtual bool read(const QJsonObject &json)=0;
    virtual bool write(QJsonObject &json) const=0;
    virtual bool read_unwrapped(const QJsonObject &json);
    virtual bool write_unwrapped(QJsonObject &json) const;

    bool unwrap(const QString &pswd);
    bool rewrap(const QString &prev_pswd, const QString &next_pswd);
    void init_wkey(const QString &pswd);

signals:
    void modified();

protected:
    inline void set_valid(bool valid=true) { m_valid=valid; }

    bool read_wrapped(const QJsonObject &json);
    bool write_wrapped(QJsonObject &json) const;

private:
    QUuid m_id;
    bool m_valid;
    bool m_wrapped;
    QString m_wkey;
    QString m_data;
    QString m_pswd;
    PicsouDBO *m_parent;
};

#include <QPointer>
#define DECL_PICSOU_MOD_OBJ_PTR(Class, ClassPtr, ClassShPtr, ClassPtrList, ClassShPtrList) \
    typedef QPointer<Class> ClassPtr; \
    typedef QSharedPointer<Class> ClassShPtr; \
    typedef QList<ClassPtr> ClassPtrList; \
    typedef QList<ClassShPtr> ClassShPtrList

#define DELETE_HASH_CONTENT(Class, hash) \
    { \
        QHash<QUuid, Class>::iterator it__=(hash).begin(); \
        while(it__!=(hash).end()) { \
            delete it__.value(); \
            it__++; \
        } \
        (hash).clear(); \
    }

#define JSON_CHECK_KEYS(list) \
    { \
        QStringList keys__=(list); \
        for(auto i__ : keys__) { \
            if(!json.contains(i__)) { \
                set_valid(false); \
                LOG_BOOL_RETURN(false); \
            } \
        } \
    }

#define JSON_READ_LIST(json, name, member, Class, parent) \
    { \
        Class *obj__; \
        QJsonArray array__=(json)[(name)].toArray(); \
        for(int i__=0; i__<array__.size(); ++i__) { \
            obj__=new Class(parent); \
            if(!obj__->read(array__[i__].toObject())) { \
                /* TRACE */ \
                set_valid(false); \
                LOG_BOOL_RETURN(false); \
            } \
            (member).insert(obj__->id(), QPointer<Class>(obj__)); \
        } \
    }

#define JSON_WRITE_LIST(json, name, list) \
    { \
        QJsonObject obj__; \
        QJsonArray array__; \
        for(int i__=0; i__<(list).size(); ++i__) { \
            obj__=QJsonObject(); \
            if(!(list)[i__]->write(obj__)) { \
                /* TRACE */ \
                LOG_BOOL_RETURN(false); \
            } \
            array__.append(obj__); \
        } \
        (json)[(name)]=array__; \
    }

#endif // PICSOUDBO_H
