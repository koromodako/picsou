#include "operation.h"
#include "paymentmethod.h"
#include "utils/macro.h"

const QString Operation::KW_AMOUNT="amount";
const QString Operation::KW_DAY="day";
const QString Operation::KW_MONTH="month";
const QString Operation::KW_YEAR="year";
const QString Operation::KW_BUDGET="budget";
const QString Operation::KW_RECIPIENT="recipient";
const QString Operation::KW_DESCRIPTION="description";
const QString Operation::KW_PAYMENT_METHOD="paymentMethod";

Operation::Operation(PicsouModelObj *parent) :
    PicsouModelObj(false, parent)
{

}

Operation::Operation(const Amount &amount,
                     const QDate &date,
                     const QString &budget,
                     const QString &recipient,
                     const QString &description,
                     const QString &payment_method,
                     PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
    _amount(amount),
    _date(date),
    _budget(budget),
    _recipient(recipient),
    _description(description),
    _payment_method(payment_method)
{

}

void Operation::update(Amount amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method)
{
    _amount=amount;
    _date=date;
    _budget=budget;
    _recipient=recipient;
    _description=description;
    _payment_method=payment_method;
    emit modified();
}


bool Operation::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>");
    static const QStringList keys=(QStringList()<<Operation::KW_AMOUNT
                                   <<Operation::KW_DAY
                                   <<Operation::KW_MONTH
                                   <<Operation::KW_YEAR
                                   <<Operation::KW_BUDGET
                                   <<Operation::KW_RECIPIENT
                                   <<Operation::KW_DESCRIPTION
                                   <<Operation::KW_PAYMENT_METHOD);
    JSON_CHECK_KEYS(keys);
    /**/
    _amount=json[KW_AMOUNT].toDouble();
    _date=QDate(json[KW_YEAR].toInt(),
                json[KW_MONTH].toInt(),
                json[KW_DAY].toInt());
    _budget=json[KW_BUDGET].toString();
    _recipient=json[KW_RECIPIENT].toString();
    _description=json[KW_DESCRIPTION].toString();
    _payment_method=json[KW_PAYMENT_METHOD].toString();
    /**/
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool Operation::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_AMOUNT]=_amount.value();
    json[KW_DAY]=_date.day();
    json[KW_MONTH]=_date.month();
    json[KW_YEAR]=_date.year();
    json[KW_BUDGET]=_budget;
    json[KW_RECIPIENT]=_recipient;
    json[KW_DESCRIPTION]=_description;
    json[KW_PAYMENT_METHOD]=_payment_method;
    /**/
    LOG_BOOL_RETURN(true);
}
