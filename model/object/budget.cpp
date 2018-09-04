#include "budget.h"

#define KEYS (QStringList() << KW_BGT_NAME \
                            << KW_BGT_AMOUNT \
                            << KW_BGT_DESCRIPTION)

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
    _name=json[KW_BGT_NAME].toString();
    _amount=json[KW_BGT_AMOUNT].toDouble();
    _description=json[KW_BGT_DESCRIPTION].toString();
    /**/
    set_valid();
end:
    return valid();
}

bool Budget::write(QJsonObject &json) const
{
    /**/
    json[KW_BGT_NAME]=_name;
    json[KW_BGT_AMOUNT]=_amount;
    json[KW_BGT_DESCRIPTION]=_description;
    /**/
    return true;
}

bool Budget::operator <(const Budget &other)
{
    return (_name < other._name);
}

