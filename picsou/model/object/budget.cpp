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
#include "budget.h"
#include "utils/macro.h"

const QString Budget::KW_NAME="name";
const QString Budget::KW_AMOUNT="amount";
const QString Budget::KW_DESCRIPTION="description";

Budget::Budget(PicsouDBO *parent) :
    PicsouDBO(false, parent)
{

}

Budget::Budget(const Amount &amount,
               const QString &name,
               const QString &description,
               PicsouDBO *parent) :
    PicsouDBO(true, parent),
    m_amount(amount),
    m_name(name),
    m_description(description)
{

}

void Budget::update(const Amount &amount,
                    const QString &name,
                    const QString &description)
{
    m_amount=amount;
    m_name=name;
    m_description=description;
    emit modified();
}

bool Budget::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>")
    static const QStringList keys=(QStringList()<<KW_NAME
                                                <<KW_AMOUNT
                                                <<KW_DESCRIPTION);
    JSON_CHECK_KEYS(keys);
    /**/
    m_name=json[KW_NAME].toString();
    m_amount=json[KW_AMOUNT].toDouble();
    m_description=json[KW_DESCRIPTION].toString();
    /**/
    set_valid();
    LOG_BOOL_RETURN(valid())
}

bool Budget::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_NAME]=m_name;
    json[KW_AMOUNT]=m_amount.value();
    json[KW_DESCRIPTION]=m_description;
    /**/
    LOG_BOOL_RETURN(true)
}

bool Budget::operator <(const Budget &other)
{
    return (m_name<other.m_name);
}

