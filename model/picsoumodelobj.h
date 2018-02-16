#ifndef PICSOUMODELOBJ_H
#define PICSOUMODELOBJ_H

#include <QUuid>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>

class PicsouModelObj
{
public:
    PicsouModelObj(bool valid);
    virtual ~PicsouModelObj();

    inline QUuid id() const { return _id; }
    inline bool valid() const { return _valid; }

    virtual bool read(const QJsonObject &json) = 0;
    virtual bool write(QJsonObject &json) const = 0;

protected:
    bool _valid;

private:
    QUuid _id;
};

#define JSON_CHECK_KEYS(list) \
    do { \
        QStringList keys=(list); \
        for(QStringList::const_iterator i=keys.constBegin(); \
            i!=keys.constEnd(); \
            i++) { \
            if(!json.contains(*i)) { \
                _valid = false; \
                goto end; \
            } \
        } \
    } while(0)

#define JSON_READ_LIST(json, name, member, Class) \
    do { \
        QJsonArray array = (json)[(name)].toArray(); \
        for(int i=0; i < array.size(); ++i) { \
            Class obj = Class(); \
            if(!obj.read(array[i].toObject())) { \
                /* TRACE */ \
                _valid = false; \
                goto end; \
            } \
            (member).insert(obj.id(), obj); \
        } \
    } while (0)

#define JSON_WRITE_LIST(json, name, list) \
    do { \
        QJsonArray array = QJsonArray(); \
        for(int i=0; i<(list).size(); ++i) { \
            QJsonObject obj = QJsonObject(); \
            if(!(list)[i].write(obj)) { \
                /* TRACE */ \
                ok = false; \
                goto end; \
            } \
            array.append(obj); \
        } \
        (json)[(name)] = array; \
    } while(0)

#endif // PICSOUMODELOBJ_H
