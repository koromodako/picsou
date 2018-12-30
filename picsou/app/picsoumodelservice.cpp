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
#include "picsoumodelservice.h"
#include "utils/macro.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "picsou.h"
#include "utils/cryptoctx.h"
#include "app/picsouapplication.h"
#include "model/object/picsoudb.h"
#include "model/converter/converter.h"

static const QString XML_ELEM_OP="operation";
static const QString XML_ATTR_YEAR="year";
static const QString XML_ATTR_MONTH="month";
static const QString XML_ATTR_DAY="day";
static const QString XML_ATTR_AMOUNT="amount";
static const QString XML_ATTR_BUDGET="budget";
static const QString XML_ATTR_RECIPIENT="recipient";
static const QString XML_ATTR_PAYMENT_METHOD="paymentMethod";
static const QString XML_ATTR_DESCRIPTION="description";

PicsouModelService::~PicsouModelService()
{
    LOG_IN_VOID();
    close_db();
    LOG_VOID_RETURN();
}

PicsouModelService::PicsouModelService(PicsouApplication *papp) :
    PicsouAbstractService(papp),
    m_db(nullptr),
    m_filename(QString()),
    m_is_db_modified(false)
{
    LOG_IN("papp="<<papp);
    LOG_VOID_RETURN();
}

bool PicsouModelService::initialize()
{
    LOG_IN_VOID();
    LOG_BOOL_RETURN(true);
}

void PicsouModelService::terminate()
{
    LOG_IN_VOID();
    close_db();
    LOG_VOID_RETURN();
}

bool PicsouModelService::new_db(QString filename,
                                QString name,
                                QString description)
{
    LOG_IN("filename="<<filename<<",name="<<name<<",description="<<description);
    if(is_db_opened()) {
        LOG_BOOL_RETURN(false);
    }
    m_db=PicsouDBShPtr(new PicsouDB(SemVer(PICSOU_DB_MAJOR,
                                        PICSOU_DB_MINOR,
                                        PICSOU_DB_PATCH),
                                 name,
                                 description));
    m_filename=filename;
    m_is_db_modified=true;
    connect(m_db.data(), &PicsouDB::modified, this, &PicsouModelService::dbo_modified);
    connect(m_db.data(), &PicsouDB::unwrapped, this, &PicsouModelService::dbo_unwrapped);
    LOG_BOOL_RETURN(true);
}

bool PicsouModelService::open_db(QString filename)
{
    LOG_IN("filename="<<filename);
    if(is_db_opened()) {
        LOG_BOOL_RETURN(false);
    }
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)) {
        LOG_BOOL_RETURN(false);
    }
    QByteArray raw=f.readAll();
    /* attempt uncompressing */
    QByteArray jdata=qUncompress(raw);
    if(jdata.isEmpty()) {
        /* ensure backward compatibility */
        jdata=raw;
    }
    /* parse json data */
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(jdata, &err);
    if(doc.isNull()) {
        LOG_CRITICAL("JSON document is NULL!");
        LOG_BOOL_RETURN(false);
    }
    SemVer db_version;
    m_db=PicsouDBShPtr(new PicsouDB);
    for(;;) {
        /* attempt to load objects from json */
        if(m_db->read(doc.object())) {
            /* everything seems fine, break and go ahead */
            break;
        }
        /* something is wrong... */
        db_version=m_db->version();
        LOG_DEBUG("valid DB version: "<<db_version.is_valid());
        LOG_DEBUG(db_version.to_str()<<"<"<<PICSOU_DB_VERSION.to_str()<<" : "<<(db_version<PICSOU_DB_VERSION));
        if(db_version.is_valid()&&db_version<PICSOU_DB_VERSION) {
            /* database version is older than currently supported, attempt conversion */
            if(Converter::convert(&doc, db_version, papp()->ui_svc())) {
                /* retry to read document */
                continue;
            }
            /* conversion failed */
        }
        LOG_CRITICAL("conversion failed or database is corrupted.");
        LOG_BOOL_RETURN(false);
    }
    /* success */
    m_filename=filename;
    connect(m_db.data(), &PicsouDB::modified, this, &PicsouModelService::dbo_modified);
    connect(m_db.data(), &PicsouDB::unwrapped, this, &PicsouModelService::dbo_unwrapped);
    LOG_BOOL_RETURN(true);
}

bool PicsouModelService::save_db()
{
    LOG_IN_VOID();
    LOG_BOOL_RETURN(save_db_as(m_filename));
}

bool PicsouModelService::save_db_as(QString filename)
{
    LOG_IN("filename="<<filename);
    if(!is_db_opened()) {
        LOG_BOOL_RETURN(false);
    }
    QJsonObject json;
    if(!m_db->write(json)) {
        LOG_BOOL_RETURN(false);
    }
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly)) {
        LOG_BOOL_RETURN(false);
    }
    QByteArray jdata=QJsonDocument(json).toJson(QJsonDocument::Compact);
    QByteArray compressed=qCompress(jdata);
    if(f.write(compressed)!=compressed.size()) {
        LOG_BOOL_RETURN(false);
    }
    f.close();
    m_filename=filename;
    m_is_db_modified=false;
    LOG_BOOL_RETURN(true);
}

bool PicsouModelService::close_db()
{
    LOG_IN_VOID();
    if(is_db_opened()) {
        m_filename.clear();
        m_is_db_modified=false;
        m_db.clear();
        LOG_BOOL_RETURN(true);
    }
    LOG_BOOL_RETURN(false);
}

bool PicsouModelService::is_db_opened()
{
    LOG_IN_VOID();
    LOG_BOOL_RETURN(!(m_db.isNull()));
}

OperationCollection PicsouModelService::load_ops(ImportExportFormat fmt,
                                                 QString filename)
{
    LOG_IN("fmt="<<fmt<<",filename="<<filename);
    QFile f(filename);
    OperationCollection ops;
    if(!f.open(QIODevice::ReadOnly)) {
        LOG_WARNING("-> failed to open file "<<f);
        return ops;
    }
    switch (fmt) {
        case CSV: ops=csv_load_ops(f); break;
        case XML: ops=xml_load_ops(f); break;
        case JSON: ops=json_load_ops(f); break;
    }
    f.close();
    LOG_DEBUG("-> ops.length="<<ops.length());
    return ops;
}

bool PicsouModelService::dump_ops(ImportExportFormat fmt,
                                  QString filename,
                                  OperationCollection ops)
{
    LOG_IN("fmt="<<fmt<<",filename="<<filename<<"ops.length="<<ops.length());
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly)) {
        LOG_BOOL_RETURN(false);
    }
    bool success=false;
    switch (fmt) {
    case CSV: success=csv_dump_ops(f, ops); break;
    case XML: success=xml_dump_ops(f, ops); break;
    case JSON: success=json_dump_ops(f, ops); break;
    }
    f.close();
    LOG_BOOL_RETURN(success);
}

UserShPtr PicsouModelService::find_user(QUuid id) const
{
    LOG_IN("id="<<id);
    UserShPtr user=m_db->find_user(id);
    LOG_DEBUG("-> user="<<user);
    return user;
}

AccountShPtr PicsouModelService::find_account(QUuid id) const
{
    LOG_IN("id="<<id);
    AccountShPtr account=m_db->find_account(id);
    LOG_DEBUG("-> account="<<account);
    return account;
}

void PicsouModelService::dbo_modified()
{
    LOG_IN_VOID();
    m_is_db_modified=true;
    emit updated(m_db);
    LOG_VOID_RETURN();
}

void PicsouModelService::dbo_unwrapped()
{
    LOG_IN_VOID();
    emit unwrapped(m_db);
    LOG_VOID_RETURN();
}

OperationCollection PicsouModelService::xml_load_ops(QFile &f)
{
    LOG_IN("&f="<<&f);
    bool ok;
    int y,m,d;
    QDate date;
    double amount;
    OperationShPtr op;
    OperationCollection ops;
    QXmlStreamReader xml(&f);
    QXmlStreamAttributes attrs;
    QXmlStreamReader::TokenType token;
    while (!xml.atEnd()) {
        switch (token=xml.readNext()) {
        case QXmlStreamReader::Invalid:
            LOG_WARNING("-> XML parser failed to parse input (token="<<token<<").");
            ops.clear();
            return ops;
        case QXmlStreamReader::StartElement:
            if(xml.name()=="operation") {
                attrs=xml.attributes();
                amount=attrs.value(XML_ATTR_AMOUNT).toDouble(&ok);
                if(!ok) {
                    LOG_WARNING("-> XML import parsing error: invalid amount.");
                    ops.clear();
                    return ops;
                }
                y=attrs.value(XML_ATTR_YEAR).toInt(&ok);
                if(!ok) {
                    LOG_WARNING("-> XML import parsing error: invalid year.");
                    ops.clear();
                    return ops;
                }
                m=attrs.value(XML_ATTR_MONTH).toInt(&ok);
                if(!ok) {
                    LOG_WARNING("-> XML import parsing error: invalid month.");
                    ops.clear();
                    return ops;
                }
                d=attrs.value(XML_ATTR_DAY).toInt(&ok);
                if(!ok) {
                    LOG_WARNING("-> XML import parsing error: invalid day.");
                    ops.clear();
                    return ops;
                }
                date=QDate(y, m, d);
                if(!date.isValid()) {
                    LOG_WARNING("-> import parsing error: invalid date.");
                    ops.clear();
                    return ops;
                }
                op=OperationShPtr(new Operation(false,
                                                amount,
                                                QDate(y, m, d),
                                                attrs.value(XML_ATTR_BUDGET).toString(),
                                                attrs.value(XML_ATTR_RECIPIENT).toString(),
                                                attrs.value(XML_ATTR_DESCRIPTION).toString(),
                                                attrs.value(XML_ATTR_PAYMENT_METHOD).toString(),
                                                nullptr));
                ops.append(op);
            }
            break;
        default:
            break;
        };

    }
    if(xml.hasError()) {
        LOG_WARNING("-> XML parser failed to parse input.");
        ops.clear();
        return ops;
    }
    LOG_DEBUG("-> ops.length="<<ops.length());
    return ops;
}

OperationCollection PicsouModelService::csv_load_ops(QFile &f)
{
    LOG_IN("&f="<<&f);
    QDate date;
    double amount=0.0;
    bool ok, instr;
    QByteArray line;
    OperationShPtr op;
    int y=0, m=0, d=0, idx;
    OperationCollection ops;
    QString buffer, budget, recipient, payment_method;
    while(!f.atEnd()) {
        line=f.readLine().trimmed();
        if(line.length()>0) {
            idx=0;
            instr=false;
            for(auto c : line) {
                switch (c) {
                case '"':
                    instr=!instr;
                    break;
                case ',':
                    if(!instr){
                        switch (idx) {
                        case 0: /* year */
                            y=buffer.toInt(&ok);
                            if(!ok) {
                                LOG_WARNING("-> CSV import parsing error: invalid year.");
                                ops.clear();
                                return ops;
                            }
                            break;
                        case 1: /* month */
                            m=buffer.toInt(&ok);
                            if(!ok) {
                                LOG_WARNING("-> CSV import parsing error: invalid month.");
                                ops.clear();
                                return ops;
                            }
                            break;
                        case 2: /* day */
                            d=buffer.toInt(&ok);
                            if(!ok) {
                                LOG_WARNING("-> CSV import parsing error: invalid day.");
                                ops.clear();
                                return ops;
                            }
                            break;
                        case 3: /* amount */
                            amount=buffer.toDouble(&ok);
                            if(!ok) {
                                LOG_WARNING("-> CSV import parsing error: invalid amount.");
                                ops.clear();
                                return ops;
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
                    buffer.append(c);
                }
            }
            if(!payment_method.isNull()) {
                date=QDate(y, m, d);
                if(!date.isValid()) {
                    LOG_WARNING("-> CSV import parsing error: invalid date.");
                    ops.clear();
                    return ops;
                }
                op=OperationShPtr(new Operation(false,
                                                amount,
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
    }
    LOG_DEBUG("-> ops.length="<<ops.length());
    return ops;
}

OperationCollection PicsouModelService::json_load_ops(QFile &f)
{
    LOG_IN("&f="<<&f);
    QByteArray line;
    OperationShPtr op;
    QJsonDocument doc;
    OperationCollection ops;
    while(!f.atEnd()) {
        line=f.readLine().trimmed();
        if(line.length()>0) {
            doc=QJsonDocument::fromJson(line);
            if(!doc.isNull()) {
                op=OperationShPtr(new Operation(nullptr));
                ops.append(op);
                if(!op->read(doc.object())) {
                    LOG_WARNING("JSON import parsing error: failed to read operation.");
                    ops.clear();
                    return ops;
                }
            }
        }
    }
    LOG_DEBUG("-> ops.length="<<ops.length());
    return ops;
}

bool PicsouModelService::xml_dump_ops(QFile &f, OperationCollection ops)
{
    LOG_IN("&f="<<&f<<",ops.length="<<ops.length());
    QXmlStreamWriter xml(&f);
    xml.writeStartDocument("1.0", true);
    xml.writeStartElement("operations");
    for(const auto &op : ops.list()) {
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
    LOG_BOOL_RETURN(true);
}

bool PicsouModelService::csv_dump_ops(QFile &f, OperationCollection ops)
{
    LOG_IN("&f="<<&f<<",ops.length="<<ops.length());
    for(const auto &op : ops.list()) {
        f.write(QString("%0,%1,%2,%3,\"%4\",\"%5\",\"%6\",\"%7\"\n").arg(
                    QString::number(op->date().year()),
                    QString::number(op->date().month()),
                    QString::number(op->date().day()),
                    op->amount().to_str(),
                    op->budget().replace('"', '\''),
                    op->recipient().replace('"', '\''),
                    op->payment_method().replace('"', '\''),
                    op->description().replace('"', '\'').replace('\n', ';'))
                .toUtf8());
    }
    LOG_BOOL_RETURN(true);
}

bool PicsouModelService::json_dump_ops(QFile &f, OperationCollection ops)
{
    LOG_IN("&f="<<&f<<",ops.length="<<ops.length());
    QJsonObject obj;
    for(const auto &op : ops.list()) {
        if(!op->write(obj)) {
            f.write(tr("-- [export error] --").toUtf8());
            LOG_BOOL_RETURN(false);
        }
        f.write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
        f.write("\n");
    }
    LOG_BOOL_RETURN(true);
}
