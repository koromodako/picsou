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
#include "amount.h"
#include "utils/macro.h"

#include <QObject>

Amount::Amount(int value) :
    m_value(value)
{

}

Amount::Amount(double value) :
    m_value(value)
{

}

Amount::Amount(const Amount &other) :
    m_value(other.m_value)
{

}

QString Amount::to_str(bool add_currency) const
{
    QString str;
    if(add_currency) {
        str=QObject::tr("$%0").arg(m_value, 0, 'f', 2);
    } else {
        str=QString("%0").arg(m_value, 0, 'f', 2);
    }
    return str;
}

Amount &Amount::operator=(const Amount &other)
{
    m_value=other.m_value;
    return *this;
}

QDebug operator<<(QDebug debug, const Amount &amount)
{
    debug<<"Amount("<<amount.value()<<")";
    return debug;
}
