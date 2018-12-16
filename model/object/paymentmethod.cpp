#include "paymentmethod.h"
#include "utils/macro.h"

const QString PaymentMethod::KW_NAME="name";

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
    LOG_IN("<QJsonObject>")
    static const QStringList keys=(QStringList()<<PaymentMethod::KW_NAME);
    JSON_CHECK_KEYS(keys);
    _name=json[KW_NAME].toString();
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool PaymentMethod::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_NAME]=_name;
    LOG_BOOL_RETURN(true);
}

bool PaymentMethod::operator <(const PaymentMethod &other)
{
    return (_name < other._name);
}
