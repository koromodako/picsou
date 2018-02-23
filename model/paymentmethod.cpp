#include "paymentmethod.h"

#define KW_NAME "name"
#define KEYS (QStringList() << KW_NAME)

PaymentMethod::PaymentMethod(PicsouModelObj *parent) :
    PicsouModelObj(false, parent)
{

}

PaymentMethod::PaymentMethod(QString name, PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
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
    set_valid();
end:
    return valid();
}

bool PaymentMethod::write(QJsonObject &json) const
{
    /**/
    json[KW_NAME] = _name;
    /**/
    return true;
}

bool PaymentMethod::operator <(const PaymentMethod &other)
{
    return (_name < other._name);
}
