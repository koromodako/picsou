#include "operation.h"

#include "paymentmethod.h"

#define KEYS \
    (QStringList() << KW_OP_AMOUNT \
                   << KW_OP_DAY \
                   << KW_OP_MONTH \
                   << KW_OP_YEAR \
                   << KW_OP_BUDGET \
                   << KW_OP_RECIPIENT \
                   << KW_OP_DESCRIPTION \
                   << KW_OP_PAYMENT_METHOD)

Operation::~Operation()
{

}

Operation::Operation(PicsouModelObj *parent) :
    PicsouModelObj(false, parent)
{

}

Operation::Operation(Amount amount,
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
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _amount=json[KW_OP_AMOUNT].toDouble();
    _date=QDate(json[KW_OP_YEAR].toInt(),
                json[KW_OP_MONTH].toInt(),
                json[KW_OP_DAY].toInt());
    _budget=json[KW_OP_BUDGET].toString();
    _recipient=json[KW_OP_RECIPIENT].toString();
    _description=json[KW_OP_DESCRIPTION].toString();
    _payment_method=json[KW_OP_PAYMENT_METHOD].toString();
    /**/
    set_valid();
end:
    return valid();
}

bool Operation::write(QJsonObject &json) const
{
    /**/
    json[KW_OP_AMOUNT]=_amount.value();
    json[KW_OP_DAY]=_date.day();
    json[KW_OP_MONTH]=_date.month();
    json[KW_OP_YEAR]=_date.year();
    json[KW_OP_BUDGET]=_budget;
    json[KW_OP_RECIPIENT]=_recipient;
    json[KW_OP_DESCRIPTION]=_description;
    json[KW_OP_PAYMENT_METHOD]=_payment_method;
    /**/
    return true;
}
