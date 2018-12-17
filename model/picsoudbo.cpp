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
#include "utils/crypto.h"

#include <QJsonDocument>

const QString PicsouDBO::KW_WKEY="wkey";
const QString PicsouDBO::KW_DATA="data";

PicsouDBO::PicsouDBO(bool valid, PicsouDBO *parent) :
    m_id(QUuid::createUuid()),
    m_valid(valid),
    m_wrapped(false),
    m_parent(parent)
{
    if(parent!=nullptr) {
        connect(this, &PicsouDBO::modified, parent, &PicsouDBO::modified);
    }
}

bool PicsouDBO::read_unwrapped(const QJsonObject &)
{
    LOG_IN("<QJsonObject>");
    LOG_CRITICAL("this method should not be called.");
    LOG_BOOL_RETURN(false);
}

bool PicsouDBO::write_unwrapped(QJsonObject &) const
{
    LOG_IN("<QJsonObject>");
    LOG_CRITICAL("this method should not be called.");
    LOG_BOOL_RETURN(false);
}

bool PicsouDBO::unwrap(const QString &pswd)
{
    LOG_IN("pswd");
    QByteArray data;
    if(!Crypto::decrypt(pswd, m_wkey, m_data, data)) {
        LOG_CRITICAL("Crypto::decrypt() operation failed.");
        LOG_BOOL_RETURN(false);
    }
    QJsonDocument doc=QJsonDocument::fromBinaryData(data);
    if(doc.isNull()) {
        LOG_CRITICAL("Crypto::decrypt() operation failed.");
        LOG_BOOL_RETURN(false);
    }
    if(!read_unwrapped(doc.object())) {
        LOG_CRITICAL("read_unwrapped() operation failed.");
        LOG_BOOL_RETURN(false);
    }
    m_pswd=pswd;
    m_wrapped=false;
    LOG_BOOL_RETURN(true);
}

bool PicsouDBO::rewrap(const QString &prev_pswd, const QString &next_pswd)
{
    LOG_IN("prev_pswd,next_pswd");
    if(!Crypto::rewrap(prev_pswd, next_pswd, m_wkey)) {
        LOG_CRITICAL("Crypto::rewrap() operation failed.");
        LOG_BOOL_RETURN(false);
    }
    if(!wrapped()) {
        /* underlying object has been unwrapped => update cached pswd */
        m_pswd=next_pswd;
    }
    emit modified();
    LOG_BOOL_RETURN(true);
}

void PicsouDBO::init_wkey(const QString &pswd)
{
    LOG_IN("pswd");
    if(!Crypto::init_wkey(pswd, m_wkey)) {
        LOG_CRITICAL("Crypto::init_wkey() operation failed.");
    }
    LOG_VOID_RETURN();
}

bool PicsouDBO::read_wrapped(const QJsonObject &json)
{
    LOG_IN("json");
    static const QStringList keys=(QStringList()<<KW_WKEY
                                                <<KW_DATA);
    JSON_CHECK_KEYS(keys);
    /* loads wrapped data but does not unwrap it */
    m_wkey=json[KW_WKEY].toString();
    m_data=json[KW_DATA].toString();
    m_wrapped=true;
    LOG_BOOL_RETURN(wrapped());
}

bool PicsouDBO::write_wrapped(QJsonObject &json) const
{
    LOG_IN("json");
    QString data;
    if(wrapped()) {
        /* underlying object has not been unwrapped => keep previous data */
        data=m_data;
    } else {
        /* underlying object has been unwrapped and might have been modified => update data */
        if(m_wkey.isNull()) {
            LOG_CRITICAL("m_wkey is null.");
            LOG_BOOL_RETURN(false);
        }
        QJsonObject wjson;
        if(!write_unwrapped(wjson)) {
            LOG_CRITICAL("write_unwrapped() operation failed.");
            LOG_BOOL_RETURN(false);
        }
        QJsonDocument doc(wjson);
        if(!Crypto::encrypt(m_pswd, m_wkey, doc.toBinaryData(), data)) {
            LOG_CRITICAL("Crypto::encrypt() operation failed.");
            LOG_BOOL_RETURN(false);
        }
    }
    json[KW_WKEY]=m_wkey;
    json[KW_DATA]=data;
    LOG_BOOL_RETURN(true);
}
