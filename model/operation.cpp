#include "operation.h"

#include "paymentmethod.h"

#define KW_AMOUNT "amount"
#define KW_DAY "day"
#define KW_MONTH "month"
#define KW_YEAR "year"
#define KW_RECIPIENT "recipient"
#define KW_DESCRIPTION "description"
#define KW_PAYMENT_METHOD "payment_method"
#define KEYS \
    (QStringList() << KW_AMOUNT << KW_DAY << KW_MONTH << KW_YEAR \
    << KW_RECIPIENT << KW_DESCRIPTION << KW_PAYMENT_METHOD)

Operation::Operation() :
    PicsouModelObj(false)
{

}

Operation::Operation(double amount,
                     QDate date,
                     QString recipient,
                     QString description,
                     const PaymentMethod *payment_method) :
    PicsouModelObj(true),
    _amount(amount),
    _date(date),
    _recipient(recipient),
    _description(description),
    _payment_method(payment_method->name())
{

}

Operation::~Operation()
{

}

bool Operation::read(const QJsonObject &json)
{
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _amount = json[KW_AMOUNT].toDouble();
    _date = QDate(json[KW_YEAR].toInt(),
                  json[KW_MONTH].toInt(),
                  json[KW_DAY].toInt());
    _recipient = json[KW_RECIPIENT].toString();
    _description = json[KW_DESCRIPTION].toString();
    _payment_method = json[KW_PAYMENT_METHOD].toString();
    /**/
    _valid = true;
end:
    return _valid;
}

bool Operation::write(QJsonObject &json) const
{
    /**/
    json[KW_AMOUNT] = _amount;
    json[KW_DAY] = _date.day();
    json[KW_MONTH] = _date.month();
    json[KW_YEAR] = _date.year();
    json[KW_RECIPIENT] = _recipient;
    json[KW_DESCRIPTION] = _description;
    json[KW_PAYMENT_METHOD] = _payment_method;
    /**/
    return true;
}

bool Operation::operator <(const Operation &other)
{
    return (_date < other._date);
}
