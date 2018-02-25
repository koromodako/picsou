#include "budget.h"

#define KW_NAME "name"
#define KW_AMOUNT "amount"
#define KW_DESCRIPTION "description"
#define KEYS (QStringList() << KW_NAME << KW_AMOUNT << KW_DESCRIPTION)

Budget::Budget(PicsouModelObj *parent) :
    PicsouModelObj(false, parent)
{

}

Budget::Budget(double amount,
               const QString &name,
               const QString &description,
               PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
    _amount(amount),
    _name(name),
    _description(description)
{

}

void Budget::update(double amount,
                    const QString &name,
                    const QString &description)
{
    _amount=amount;
    _name=name;
    _description=description;
    emit modified();
}

Budget::~Budget()
{

}

bool Budget::read(const QJsonObject &json)
{
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name=json[KW_NAME].toString();
    _amount=json[KW_AMOUNT].toDouble();
    _description=json[KW_DESCRIPTION].toString();
    /**/
    set_valid();
end:
    return valid();
}

bool Budget::write(QJsonObject &json) const
{
    /**/
    json[KW_NAME]=_name;
    json[KW_AMOUNT]=_amount;
    json[KW_DESCRIPTION]=_description;
    /**/
    return true;
}

bool Budget::operator <(const Budget &other)
{
    return (_name < other._name);
}

