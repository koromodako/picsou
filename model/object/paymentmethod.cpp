#include "paymentmethod.h"
#include "utils/macro.h"

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
    LOG_IN("<QJsonObject>")
    JSON_CHECK_KEYS(KEYS);
    _name=json[KW_PM_NAME].toString();
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool PaymentMethod::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_PM_NAME]=_name;
    LOG_BOOL_RETURN(true);
}

bool PaymentMethod::operator <(const PaymentMethod &other)
{
    return (_name < other._name);
}
