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
#include "picsoudbo.h"
#include "utils/macro.h"
#include "utils/cryptoctx.h"

#include <QJsonDocument>

const QString PicsouDBO::KW_WDAT="data";
const QString PicsouDBO::KW_WKEY="key";
const QString PicsouDBO::KW_WSALT="salt";

PicsouDBO::PicsouDBO(bool valid, PicsouDBO *parent) :
    m_id(QUuid::createUuid()),
    m_valid(valid),
    m_parent(parent)
{
    if(parent!=nullptr) {
        connect(this, &PicsouDBO::modified, parent, &PicsouDBO::modified);
        connect(this, &PicsouDBO::unwrapped, parent, &PicsouDBO::unwrapped);
    }
}

bool PicsouDBO::read_unwrapped(const QJsonObject &)
{
    LOG_IN("<QJsonObject>")
    LOG_CRITICAL("this method should not be called.")
    LOG_BOOL_RETURN(false)
}

bool PicsouDBO::write_unwrapped(QJsonObject &) const
{
    LOG_IN("<QJsonObject>")
    LOG_CRITICAL("this method should not be called.")
    LOG_BOOL_RETURN(false)
}

bool PicsouDBO::unwrap(const QString &pswd)
{
    LOG_IN("pswd")
    QByteArray data;
    if(!m_wctx.unwrap(pswd, m_wdat, data)) {
        LOG_CRITICAL("CryptoCtx::unwrap() operation failed.")
        LOG_BOOL_RETURN(false)
    }
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(data, &err);
    if(doc.isNull()) {
        LOG_CRITICAL("failed to parse JSON: "<<err.errorString())
        LOG_BOOL_RETURN(false)
    }
    if(!read_unwrapped(doc.object())) {
        LOG_CRITICAL("read_unwrapped() operation failed.")
        LOG_BOOL_RETURN(false)
    }
    emit unwrapped();
    LOG_BOOL_RETURN(true)
}

void PicsouDBO::init_wkey(const QString &pswd)
{
    LOG_IN("pswd")
    if(!m_wctx.init(pswd)) {
        LOG_CRITICAL("CryptoCtx::init() operation failed.")
    }
    LOG_VOID_RETURN()
}

bool PicsouDBO::rewrap(const QString &prev_pswd, const QString &next_pswd)
{
    LOG_IN("prev_pswd,next_pswd")
    if(!m_wctx.rewrap(prev_pswd, next_pswd)) {
        LOG_CRITICAL("CryptoCtx::rewrap() operation failed.")
        LOG_BOOL_RETURN(false)
    }
    emit modified();
    LOG_BOOL_RETURN(true)
}

bool PicsouDBO::read_wrapped(const QJsonObject &json)
{
    LOG_IN("json")
    static const QStringList keys=(QStringList()<<KW_WDAT<<KW_WKEY<<KW_WSALT);
    JSON_CHECK_KEYS(keys);
    /* loads wrapped data but does not unwrap it */
    m_wdat=json[KW_WDAT].toString();
    m_wctx=CryptoCtx(json[KW_WKEY].toString(),
                     json[KW_WSALT].toString());
    LOG_BOOL_RETURN(wrapped())
}

bool PicsouDBO::write_wrapped(QJsonObject &json) const
{
    LOG_IN("json")
    QString wrapped_data;
    if(wrapped()) {
        /* underlying object has not been unwrapped => keep previous data */
        wrapped_data=m_wdat;
    } else {
        /* underlying object has been unwrapped and might have been modified => update data */
        QJsonObject wjson;
        if(!write_unwrapped(wjson)) {
            LOG_CRITICAL("write_unwrapped() operation failed.")
            LOG_BOOL_RETURN(false)
        }
        QJsonDocument wdoc(wjson);
        if(!m_wctx.wrap(wdoc.toJson(QJsonDocument::Compact), wrapped_data)) {
            LOG_CRITICAL("CryptoCtx::wrap() operation failed.")
            LOG_BOOL_RETURN(false)
        }
    }
    json[KW_WDAT]=wrapped_data;
    json[KW_WKEY]=m_wctx.wkey();
    json[KW_WSALT]=m_wctx.wsalt();
    LOG_BOOL_RETURN(true)
}
