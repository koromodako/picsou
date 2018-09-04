#include "picsoumodelservice.h"
#include "utils/macro.h"

#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "picsou.h"
#include "model/picsoudb.h"
#include "model/converter/converter.h"

#define XML_ELEM_OP "operation"
#define XML_ATTR_YEAR "year"
#define XML_ATTR_MONTH "month"
#define XML_ATTR_DAY "day"
#define XML_ATTR_AMOUNT "amount"
#define XML_ATTR_BUDGET "budget"
#define XML_ATTR_RECIPIENT "recipient"
#define XML_ATTR_PAYMENT_METHOD "paymentMethod"
#define XML_ATTR_DESCRIPTION "description"

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

    _db=PicsouDBPtr(new PicsouDB(SemVer(PICSOU_DB_MAJOR,
                                        PICSOU_DB_MINOR,
                                        PICSOU_DB_PATCH),
                                 name,
                                 description));
    _filename=filename;
    _is_db_modified=true;

    connect(_db, &PicsouDB::modified, this, &PicsouModelService::notify_ui);

    success=true;
end:
    return success;
}

bool PicsouModelService::open_db(QString filename)
{
    bool success;
    SemVer db_version;
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
        LOG_CRITICAL("JSON document is NULL!");
        success=false; goto end;
    }

    _db=PicsouDBPtr(new PicsouDB);

    for(;;) {
        if(_db->read(doc.object())) {
            break;
        }
        db_version=_db->version();
        LOG_DEBUG("valid DB version: " << db_version.is_valid());
        LOG_DEBUG(db_version.to_str() << " < " << PICSOU_DB_VERSION.to_str() << " : " << (db_version<PICSOU_DB_VERSION));
        if(db_version.is_valid()&&db_version<PICSOU_DB_VERSION) {
            if(Converter::convert(&doc, db_version)) {
                continue; /* retry to read document */
            }
            /* conversion failed */
        }
        success=false; goto end;
    }

    _filename=filename;

    connect(_db, &PicsouDB::modified, this, &PicsouModelService::notify_ui);

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

    json_data=QJsonDocument(json).toJson();

    if(f.write(json_data)!=json_data.size()) {
        success=false; goto end;
    }

    f.close();
    _filename=filename;
    _is_db_modified=false;
    success=true;
end:
    return success;
}

bool PicsouModelService::close_db()
{
    bool success=false;

    if(is_db_opened()) {
        _filename.clear();
        _is_db_modified=false;
        delete _db;
        success=true;
    }

    return success;
}

bool PicsouModelService::is_db_opened()
{
    return (!(_db.isNull()));
}

OperationCollection PicsouModelService::load_ops(ImportExportFormat fmt,
                                                 QString filename)
{
    QFile f(filename);
    OperationCollection ops;

    if(!f.open(QIODevice::ReadOnly)) {
        goto end;
    }

    switch (fmt) {
        case CSV: ops=csv_load_ops(f); break;
        case XML: ops=xml_load_ops(f); break;
        case JSON: ops=json_load_ops(f); break;
    }

    f.close();
end:
    return ops;
}

bool PicsouModelService::dump_ops(ImportExportFormat fmt,
                                  QString filename,
                                  OperationCollection ops)
{
    bool success=false;
    QFile f(filename);

    if(!f.open(QIODevice::WriteOnly)) {
        success=false; goto end;
    }

    switch (fmt) {
    case CSV: success=csv_dump_ops(f, ops); break;
    case XML: success=xml_dump_ops(f, ops); break;
    case JSON: success=json_dump_ops(f, ops); break;
    }

    f.close();
end:
    return success;
}

UserPtr PicsouModelService::find_user(QUuid id) const
{
    return _db->find_user(id);
}

AccountPtr PicsouModelService::find_account(QUuid id) const
{
    return _db->find_account(id);
}

void PicsouModelService::notify_ui()
{
    _is_db_modified=true;
    emit updated(_db);
}

OperationCollection PicsouModelService::xml_load_ops(QFile &f)
{
    bool ok;
    int y,m,d;
    QDate date;
    double amount;
    OperationPtr op;
    OperationCollection ops;
    QXmlStreamReader xml(&f);
    QXmlStreamAttributes attrs;
    QXmlStreamReader::TokenType token;

    while (!xml.atEnd()) {

        switch (token=xml.readNext()) {
        case QXmlStreamReader::Invalid:
            goto error;
        case QXmlStreamReader::StartElement:
            if(xml.name()=="operation") {
                attrs=xml.attributes();

                amount=attrs.value(XML_ATTR_AMOUNT).toDouble(&ok);
                if(!ok) {
                    LOG_WARNING("XML import parsing error: invalid amount.");
                    goto error;
                }
                y=attrs.value(XML_ATTR_YEAR).toInt(&ok);
                if(!ok) {
                    LOG_WARNING("XML import parsing error: invalid year.");
                    goto error;
                }
                m=attrs.value(XML_ATTR_MONTH).toInt(&ok);
                if(!ok) {
                    LOG_WARNING("XML import parsing error: invalid month.");
                    goto error;
                }
                d=attrs.value(XML_ATTR_DAY).toInt(&ok);
                if(!ok) {
                    LOG_WARNING("XML import parsing error: invalid day.");
                    goto error;
                }
                date=QDate(y, m, d);
                if(!date.isValid()) {
                    LOG_WARNING("import parsing error: invalid date.");
                    goto error;
                }
                op=OperationPtr(new Operation(amount,
                                              QDate(y, m, d),
                                              attrs.value(XML_ATTR_BUDGET).toString(),
                                              attrs.value(XML_ATTR_RECIPIENT).toString(),
                                              attrs.value(XML_ATTR_DESCRIPTION).toString(),
                                              attrs.value(XML_ATTR_PAYMENT_METHOD).toString(),
                                              nullptr));
                ops.append(op);
            }
            break;
        default: break;
        };

    }

    if(!xml.hasError()) {
        goto end;
    }

error:
    ops.clear();
end:
    return ops;
}

OperationCollection PicsouModelService::csv_load_ops(QFile &f)
{
    QDate date;
    double amount;
    bool ok, instr;
    QByteArray line;
    OperationPtr op;
    int y, m, d, idx;
    OperationCollection ops;
    QByteArray::const_iterator c;
    QString buffer, budget, recipient, payment_method;

    while(!f.atEnd()) {
        line=f.readLine().trimmed();
        if(line.length()>0) {
            idx=0;
            instr=false;

            for(c=line.constBegin(); c!=line.constEnd(); c++) {
                switch (*c) {
                case '"':
                    instr=!instr;
                    break;
                case ',':
                    if(!instr){
                        switch (idx) {
                        case 0: /* year */
                            y=buffer.toInt(&ok);
                            if(!ok) {
                                LOG_WARNING("CSV import parsing error: invalid year.");
                                goto error;
                            }
                            break;
                        case 1: /* month */
                            m=buffer.toInt(&ok);
                            if(!ok) {
                                LOG_WARNING("CSV import parsing error: invalid month.");
                                goto error;
                            }
                            break;
                        case 2: /* day */
                            d=buffer.toInt(&ok);
                            if(!ok) {
                                LOG_WARNING("CSV import parsing error: invalid day.");
                                goto error;
                            }
                            break;
                        case 3: /* amount */
                            amount=buffer.toDouble(&ok);
                            if(!ok) {
                                LOG_WARNING("CSV import parsing error: invalid amount.");
                                goto error;
                            }
                            break;
                        case 4: /* budget */
                            budget=buffer;
                            break;
                        case 5: /* recipient */
                            recipient=buffer;
                            break;
                        case 6: /* payment method */
                            payment_method=buffer;
                            break;
                        }
                        buffer.clear();
                        idx++;
                    }
                    break;
                default:
                    buffer.append(*c);
                }
            }
            date=QDate(y, m, d);
            if(!date.isValid()) {
                LOG_WARNING("CSV import parsing error: invalid date.");
                goto error;
            }
            op=OperationPtr(new Operation(amount,
                                          QDate(y, m, d),
                                          budget,
                                          recipient,
                                          buffer.replace(';', '\n'),
                                          payment_method,
                                          nullptr));
            ops.append(op);
            buffer.clear();
        }
    }
    goto end;

error:
    ops.clear();
end:
    return ops;
}

OperationCollection PicsouModelService::json_load_ops(QFile &f)
{
    QByteArray line;
    OperationPtr op;
    QJsonDocument doc;
    OperationCollection ops;

    while(!f.atEnd()) {
        line=f.readLine().trimmed();
        if(line.length()>0) {
            doc=QJsonDocument::fromJson(line);
            if(!doc.isNull()) {
                op=OperationPtr(new Operation(nullptr));
                ops.append(op);
                if(!op->read(doc.object())) {
                    LOG_WARNING("JSON import parsing error: failed to read operation.");
                    goto error;
                }
            }
        }
    }
    goto end;

error:
    ops.clear();
end:
    return ops;
}

bool PicsouModelService::xml_dump_ops(QFile &f, OperationCollection ops)
{
    bool success;
    QXmlStreamWriter xml(&f);
    xml.writeStartDocument("1.0", true);
    xml.writeStartElement("operations");
    foreach (OperationPtr op, ops.list()) {
        xml.writeEmptyElement(XML_ELEM_OP);
        xml.writeAttribute(XML_ATTR_YEAR, QString::number(op->date().year()));
        xml.writeAttribute(XML_ATTR_MONTH, QString::number(op->date().month()));
        xml.writeAttribute(XML_ATTR_DAY, QString::number(op->date().day()));
        xml.writeAttribute(XML_ATTR_AMOUNT, op->amount().to_str());
        xml.writeAttribute(XML_ATTR_BUDGET, op->budget());
        xml.writeAttribute(XML_ATTR_RECIPIENT, op->recipient());
        xml.writeAttribute(XML_ATTR_PAYMENT_METHOD, op->payment_method());
        xml.writeAttribute(XML_ATTR_DESCRIPTION, op->description());
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    success=true;
//end:
    return success;
}

bool PicsouModelService::csv_dump_ops(QFile &f, OperationCollection ops)
{
    bool success;
    foreach (OperationPtr op, ops.list()) {
        f.write(QString("%0,%1,%2,%3,\"%4\",\"%5\",\"%6\",\"%7\"\n").arg(
                    QString::number(op->date().year()),
                    QString::number(op->date().month()),
                    QString::number(op->date().day()),
                    op->amount().to_str(true),
                    op->budget().replace('"', '\''),
                    op->recipient().replace('"', '\''),
                    op->payment_method().replace('"', '\''),
                    op->description().replace('"', '\'').replace('\n', ';'))
                .toUtf8());
    }
    success=true;
//end:
    return success;
}

bool PicsouModelService::json_dump_ops(QFile &f, OperationCollection ops)
{
    bool success;
    QJsonObject obj;
    foreach (OperationPtr op, ops.list()) {
        if(!op->write(obj)) {
            f.write(tr("-- [export error] --").toUtf8());
            success=false; goto end;
        }
        f.write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
        f.write("\n");
    }
    success=true;
end:
    return success;
}
