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
#ifndef PICSOUMODELOBJ_H
#define PICSOUMODELOBJ_H

#include <QUuid>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>

class PicsouModelObj : public QObject
{
    Q_OBJECT
public:
    PicsouModelObj(bool valid, PicsouModelObj *parent);
    virtual ~PicsouModelObj();

    inline QUuid id() const { return _id; }
    inline bool valid() const { return _valid; }

    inline void set_parent(PicsouModelObj *parent) { _parent=parent; }

    virtual bool read(const QJsonObject &json)=0;
    virtual bool write(QJsonObject &json) const=0;

signals:
    void modified();

protected:
    void set_valid(bool valid=true) { _valid=valid; }

private:
    QUuid _id;
    bool _valid;
    PicsouModelObj *_parent;
};

#include <QPointer>
#define DECL_PICSOU_MOD_OBJ_PTR(Class, ClassPtr, ClassShPtr, ClassPtrList, ClassShPtrList) \
    typedef QPointer<Class> ClassPtr; \
    typedef QSharedPointer<Class> ClassShPtr; \
    typedef QList<ClassPtr> ClassPtrList; \
    typedef QList<ClassShPtr> ClassShPtrList

#define DELETE_HASH_CONTENT(Class, hash) \
    { \
        QHash<QUuid, Class>::iterator it=(hash).begin(); \
        while(it!=(hash).end()) { \
            delete it.value(); \
            it++; \
        } \
        (hash).clear(); \
    }

#define JSON_CHECK_KEYS(list) \
    { \
        QStringList __keys=(list); \
        for(auto __i : __keys) { \
            if(!json.contains(__i)) { \
                set_valid(false); \
                LOG_BOOL_RETURN(false); \
            } \
        } \
    }

#define JSON_READ_LIST(json, name, member, Class, parent) \
    { \
        Class *__obj; \
        QJsonArray __array=(json)[(name)].toArray(); \
        for(int __i=0; __i<__array.size(); ++__i) { \
            __obj=new Class(parent); \
            if(!__obj->read(__array[__i].toObject())) { \
                /* TRACE */ \
                set_valid(false); \
                LOG_BOOL_RETURN(false); \
            } \
            (member).insert(__obj->id(), QPointer<Class>(__obj)); \
        } \
    }

#define JSON_WRITE_LIST(json, name, list) \
    { \
        QJsonObject __obj; \
        QJsonArray __array; \
        for(int __i=0; __i<(list).size(); ++__i) { \
            __obj=QJsonObject(); \
            if(!(list)[__i]->write(__obj)) { \
                /* TRACE */ \
                LOG_BOOL_RETURN(false); \
            } \
            __array.append(__obj); \
        } \
        (json)[(name)]=__array; \
    }

#endif // PICSOUMODELOBJ_H
