#include "picsoudb.h"

#include <QFile>
#include <QJsonArray>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonParseError>

PicsouDB::PicsouDB() :
    PicsouModelObj(false)
{

}

PicsouDB::PicsouDB(QString fpath,
                   uint version_major,
                   uint version_minor,
                   QString description) :
    PicsouModelObj(true),
    _fpath(fpath),
    _version(version_major, version_minor),
    _description(description)
{

}

PicsouDB::~PicsouDB()
{

}

bool PicsouDB::open()
{
    bool ok;
    QJsonParseError err;
    QJsonDocument doc;
    QFile f(_fpath);
    /**/
    if(!f.open(QIODevice::ReadOnly)) {
        /* TRACE */
        ok = false;
        goto end;
    }
    doc = QJsonDocument::fromJson(f.readAll(), &err);
    f.close();
    if(doc.isNull()) {
        /* TRACE err.errorString() */
        ok = false;
        goto end;
    }
    /**/
    ok = read(doc.object());
end:
    return ok;
}

bool PicsouDB::save()
{
    bool ok;
    QJsonObject obj;
    QJsonDocument doc;
    QFile f(_fpath);
    /**/
    if(!f.open(QIODevice::WriteOnly)) {
        /* TRACE */
        ok = false;
        goto end;
    }
    if(!write(obj)) {
        /* TRACE */
        ok = false;
        goto end;
    }
    doc = QJsonDocument(obj);
#ifdef QT_DEBUG
    f.write(doc.toJson(QJsonDocument::Indented));
#else
    f.write(doc.toJson(QJsonDocument::Compact));
#endif
    f.close();
    /**/
    ok = true;
end:
    return ok;
}

bool PicsouDB::remove_user(QUuid id)
{
    switch (_users.remove(id)) {
        case 0: /* TRACE */ return false;
        case 1: return true;
        default: /* TRACE */ return false;
    }
}

QString vers2str(QPair<uint, uint> version)
{
    return QString("%0.%1").arg(version.first).arg(version.second);
}

QPair<uint,uint> str2vers(QString version_str)
{
    bool ok;
    QPair<uint,uint> version(-1, -1);
    QStringList parts = version_str.split('.');
    /**/
    if(parts.length()!=2) {
        /* TRACE */
        goto end;
    }
    version.first = parts.first().toUInt(&ok);
    if(!ok) {
        /* TRACE */
        goto end;
    }
    version.second = parts.last().toUInt(&ok);
    if(!ok) {
        /* TRACE */
        goto end;
    }
    /**/
end:
    return version;
}

bool PicsouDB::read(const QJsonObject &json)
{
    User user;
    QJsonArray array;
    /**/
    if(!json.contains("version") ||
       !json.contains("description") ||
       !json.contains("users")) {
        /* TRACE */
        _valid = false;
        goto end;
    }
    _version = str2vers(json["version"].toString());
    _description = json["description"].toString();
    JSON_READ_LIST(json, "users", _users, User)
    /**/
    _valid = true;
end:
    return _valid;
}

bool PicsouDB::write(QJsonObject &json) const
{
    bool ok;
    QJsonObject obj;
    QJsonArray array;
    /**/
    json["version"] = vers2str(_version);
    json["description"] = _description;
    JSON_WRITE_LIST(json, "users", _users.values());
    /**/
    ok = true;
end:
    return ok;
}
