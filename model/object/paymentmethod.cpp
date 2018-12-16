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
#include "paymentmethod.h"
#include "utils/macro.h"

const QString PaymentMethod::KW_NAME="name";

PaymentMethod::PaymentMethod(PicsouModelObj *parent) :
    PicsouModelObj(false, parent)
{

}

PaymentMethod::PaymentMethod(const QString &name, PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
    _name(name)
{

}

void PaymentMethod::update(const QString &name)
{
    _name=name;
    emit modified();
}

bool PaymentMethod::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>")
    static const QStringList keys=(QStringList()<<PaymentMethod::KW_NAME);
    JSON_CHECK_KEYS(keys);
    _name=json[KW_NAME].toString();
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool PaymentMethod::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_NAME]=_name;
    LOG_BOOL_RETURN(true);
}

bool PaymentMethod::operator <(const PaymentMethod &other)
{
    return (_name<other._name);
}
