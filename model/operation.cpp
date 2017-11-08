#include "operation.h"

Operation::Operation() :
    PicsouModelObj(false)
{

}

Operation::Operation(double amount,
                     QDate date,
                     QString description,
                     QString payment_method) :
    PicsouModelObj(true),
    _amount(amount),
    _date(date),
    _description(description),
    _payment_method(payment_method)
{

}

Operation::~Operation()
{

}

bool Operation::read(const QJsonObject &json)
{
    /**/
    if(!json.contains("amount") ||
       !json.contains("day") ||
       !json.contains("month") ||
       !json.contains("year") ||
       !json.contains("description") ||
       !json.contains("payment_method")) {
        _valid = false;
        goto end;
    }
    _amount = json["amount"].toDouble();
    _date = QDate(json["year"].toInt(), json["month"].toInt(), json["day"].toInt());
    _description = json["description"].toString();
    _payment_method = json["payment_method"].toString();
    /**/
    _valid = true;
end:
    return _valid;
}

bool Operation::write(QJsonObject &json) const
{
    /**/
    json["amount"] = _amount;
    json["day"] = _date.day();
    json["month"] = _date.month();
    json["year"] = _date.year();
    json["description"] = _description;
    json["payment_method"] = _payment_method;
    /**/
    return true;
}
