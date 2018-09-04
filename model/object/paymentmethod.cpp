#include "paymentmethod.h"

#define KEYS (QStringList() << KW_PM_NAME)

PaymentMethod::~PaymentMethod()
{

}

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
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name=json[KW_PM_NAME].toString();
    /**/
    set_valid();
end:
    return valid();
}

bool PaymentMethod::write(QJsonObject &json) const
{
    /**/
    json[KW_PM_NAME]=_name;
    /**/
    return true;
}

bool PaymentMethod::operator <(const PaymentMethod &other)
{
    return (_name < other._name);
}
