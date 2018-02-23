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

    virtual bool read(const QJsonObject &json) = 0;
    virtual bool write(QJsonObject &json) const = 0;

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
#define DECL_PICSOU_MOD_OBJ_PTR(Class, ClassPtr) \
    typedef QPointer<Class> ClassPtr

#define DELETE_HASH_CONTENT(Class, hash) \
   do { \
        QHash<QUuid, Class>::iterator it=(hash).begin(); \
        while(it!=(hash).end()) { \
            delete it.value(); \
        } \
        (hash).clear(); \
    } while(0)

#define JSON_CHECK_KEYS(list) \
    do { \
        QStringList keys=(list); \
        for(QStringList::const_iterator i=keys.constBegin(); \
            i!=keys.constEnd(); \
            i++) { \
            if(!json.contains(*i)) { \
                set_valid(false); \
                goto end; \
            } \
        } \
    } while(0)

#define JSON_READ_LIST(json, name, member, Class, parent) \
    do { \
        QJsonArray array = (json)[(name)].toArray(); \
        for(int i=0; i < array.size(); ++i) { \
            Class *obj = new Class(parent); \
            if(!obj->read(array[i].toObject())) { \
                /* TRACE */ \
                set_valid(false); \
                goto end; \
            } \
            (member).insert(obj->id(), QPointer<Class>(obj)); \
        } \
    } while (0)

#define JSON_WRITE_LIST(json, name, list) \
    do { \
        QJsonArray array = QJsonArray(); \
        for(int i=0; i<(list).size(); ++i) { \
            QJsonObject obj = QJsonObject(); \
            if(!(list)[i]->write(obj)) { \
                /* TRACE */ \
                ok = false; \
                goto end; \
            } \
            array.append(obj); \
        } \
        (json)[(name)] = array; \
    } while(0)

#endif // PICSOUMODELOBJ_H
