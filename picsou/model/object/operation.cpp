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
#include "operation.h"
#include "paymentmethod.h"
#include "utils/macro.h"

const QString Operation::KW_AMOUNT="amount";
const QString Operation::KW_DAY="day";
const QString Operation::KW_MONTH="month";
const QString Operation::KW_YEAR="year";
const QString Operation::KW_BUDGET="budget";
const QString Operation::KW_VERIFIED="verified";
const QString Operation::KW_RECIPIENT="recipient";
const QString Operation::KW_DESCRIPTION="description";
const QString Operation::KW_PAYMENT_METHOD="paymentMethod";

Operation::Operation(PicsouDBO *parent) :
    PicsouDBO(false, parent)
{

}

Operation::Operation(bool verified,
                     const Amount &amount,
                     const QDate &date,
                     const QString &budget,
                     const QString &recipient,
                     const QString &description,
                     const QString &payment_method,
                     PicsouDBO *parent) :
    PicsouDBO(true, parent),
    m_verified(verified),
    m_amount(amount),
    m_date(date),
    m_budget(budget),
    m_recipient(recipient),
    m_description(description),
    m_payment_method(payment_method)
{

}

void Operation::update(bool verified,
                       Amount amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method)
{
    m_verified=verified;
    m_amount=amount;
    m_date=date;
    m_budget=budget;
    m_recipient=recipient;
    m_description=description;
    m_payment_method=payment_method;
    emit modified();
}


bool Operation::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>");
    static const QStringList keys=(QStringList()<<KW_AMOUNT
                                                <<KW_DAY
                                                <<KW_MONTH
                                                <<KW_YEAR
                                                <<KW_BUDGET
                                                <<KW_RECIPIENT
                                                <<KW_DESCRIPTION
                                                <<KW_PAYMENT_METHOD);
    JSON_CHECK_KEYS(keys);
    /**/
    m_amount=json[KW_AMOUNT].toDouble();
    m_date=QDate(json[KW_YEAR].toInt(),
                json[KW_MONTH].toInt(),
                json[KW_DAY].toInt());
    m_budget=json[KW_BUDGET].toString();
    m_recipient=json[KW_RECIPIENT].toString();
    m_description=json[KW_DESCRIPTION].toString();
    m_payment_method=json[KW_PAYMENT_METHOD].toString();
    if(json.contains(KW_VERIFIED)) {
        m_verified=json[KW_VERIFIED].toBool();
    }
    /**/
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool Operation::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>");
    json[KW_AMOUNT]=m_amount.value();
    json[KW_DAY]=m_date.day();
    json[KW_MONTH]=m_date.month();
    json[KW_YEAR]=m_date.year();
    json[KW_BUDGET]=m_budget;
    json[KW_RECIPIENT]=m_recipient;
    json[KW_DESCRIPTION]=m_description;
    json[KW_PAYMENT_METHOD]=m_payment_method;
    json[KW_VERIFIED]=m_verified;
    /**/
    LOG_BOOL_RETURN(true);
}
