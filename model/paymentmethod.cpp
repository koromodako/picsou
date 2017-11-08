#include "paymentmethod.h"

PaymentMethod::PaymentMethod() :
    PicsouModelObj(false)
{

}

PaymentMethod::PaymentMethod(QString name) :
    PicsouModelObj(true),
    _name(name)
{

}

PaymentMethod::~PaymentMethod()
{

}

bool PaymentMethod::read(const QJsonObject &json)
{
    /**/
    if(!json.contains("name")) {
        /* TRACE */
        _valid = false;
        goto end;
    }
    _name = json["name"].toString();
    /**/
    _valid = true;
end:
    return _valid;
}

bool PaymentMethod::write(QJsonObject &json) const
{
    /**/
    json["name"] = _name;
    /**/
    return true;
}
