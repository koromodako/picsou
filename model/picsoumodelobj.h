#ifndef PICSOUMODELOBJ_H
#define PICSOUMODELOBJ_H

#include <QUuid>
#include <QJsonObject>

class PicsouModelObj
{
public:
    PicsouModelObj(bool valid);
    virtual ~PicsouModelObj();

    inline QUuid id() { return _id; }
    inline bool valid() { return _valid; }

    virtual bool read(const QJsonObject &json) = 0;
    virtual bool write(QJsonObject &json) const = 0;

protected:
    bool _valid;

private:
    QUuid _id;
};

#define JSON_READ_LIST(json, name, member, Class) \
    array = (json)[(name)].toArray(); \
    for(int i=0; i < array.size(); ++i) { \
        Class obj = Class(); \
        if(!obj.read(array[i].toObject())) { \
            /* TRACE */ \
            _valid = false; \
            goto end; \
        } \
        (member).insert(obj.id(), obj); \
    }

#define JSON_WRITE_LIST(json, name, list) \
    array = QJsonArray(); \
    for(int i=0; i<(list).size(); ++i) { \
        obj = QJsonObject(); \
        if(!(list)[i].write(obj)) { \
            /* TRACE */ \
            ok = false; \
            goto end; \
        } \
        array.append(obj); \
    } \
    (json)[(name)] = array

#endif // PICSOUMODELOBJ_H
