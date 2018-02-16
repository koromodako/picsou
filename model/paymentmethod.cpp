#include "paymentmethod.h"

#define KW_NAME "name"
#define KEYS (QStringList() << KW_NAME)

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
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name = json[KW_NAME].toString();
    /**/
    _valid = true;
end:
    return _valid;
}

bool PaymentMethod::write(QJsonObject &json) const
{
    /**/
    json[KW_NAME] = _name;
    /**/
    return true;
}
