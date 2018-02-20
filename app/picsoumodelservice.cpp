#include "picsoumodelservice.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

#include "picsou.h"
#include "model/picsoudb.h"

PicsouModelService::~PicsouModelService()
{
    close_db();
}

PicsouModelService::PicsouModelService(PicsouApplication *papp) :
    PicsouAbstractService(papp),
    _db(nullptr),
    _filename(QString()),
    _is_db_modified(false)
{

}

bool PicsouModelService::initialize()
{
    return true;
}

void PicsouModelService::terminate()
{
    close_db();
}

bool PicsouModelService::new_db(QString filename,
                                QString name,
                                QString description)
{
    bool success;

    if(is_db_opened()) {
        success=false; goto end;
    }

    _db=new PicsouDB(PICSOU_DB_MAJOR, PICSOU_DB_MINOR, name, description);
    _filename=filename;

    success=true;
end:
    return success;
}

bool PicsouModelService::open_db(QString filename)
{
    bool success;
    QJsonDocument doc;
    QJsonParseError err;
    QFile f(filename);

    if(is_db_opened()) {
        success=false; goto end;
    }

    if(!f.open(QIODevice::ReadOnly)) {
        success=false; goto end;
    }

    doc=QJsonDocument::fromJson(f.readAll(), &err);

    if(doc.isNull()) {
        /* LOG err.errorString() */
        success=false; goto end;
    }

    _db=new PicsouDB;

    if(!_db->read(doc.object())) {
        success=false; goto end;
    }

    _filename = filename;
    success=true;
end:
    return success;
}

bool PicsouModelService::save_db()
{
    return save_db_as(_filename);
}

bool PicsouModelService::save_db_as(QString filename)
{
    bool success;
    QJsonObject json;
    QByteArray json_data;
    QFile f(filename);

    if(!is_db_opened()) {
        success=false; goto end;
    }

    if(!_db->write(json)) {
        success=false; goto end;
    }

    if(!f.open(QIODevice::WriteOnly)) {
        success=false; goto end;
    }

    json_data = QJsonDocument(json).toJson();

    if(f.write(json_data)!=json_data.size()) {
        success=false; goto end;
    }

    f.close();
    _filename = filename;
    success=true;
end:
    return success;
}

bool PicsouModelService::close_db()
{
    bool success;

    if(is_db_opened()) {
        _filename.clear();
        delete _db; _db=nullptr;
        success=true; goto end;
    }

    success=false;
end:
    return success;
}

bool PicsouModelService::is_db_opened()
{
    return (_db!=nullptr);
}
