#include "picsoudb.h"

#define KW_VERSION "version"
#define KW_DESCRIPTION "description"
#define KW_USERS "users"
#define KEYS (QStringList() << KW_VERSION << KW_DESCRIPTION << KW_USERS)

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
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _version = str2vers(json[KW_VERSION].toString());
    _description = json[KW_DESCRIPTION].toString();
    JSON_READ_LIST(json, KW_USERS, _users, User);
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
    json[KW_VERSION] = vers2str(_version);
    json[KW_DESCRIPTION] = _description;
    JSON_WRITE_LIST(json, KW_USERS, _users.values());
    /**/
    ok = true;
end:
    return ok;
}
