#include "budget.h"

#define KW_NAME "name"
#define KW_AMOUNT "amount"
#define KW_DESCRIPTION "description"
#define KEYS (QStringList() << KW_NAME << KW_AMOUNT << KW_DESCRIPTION)

Budget::Budget() :
    PicsouModelObj(false)
{

}

Budget::Budget(double amount, QString name, QString description) :
    PicsouModelObj(true),
    _amount(amount),
    _name(name),
    _description(description)
{

}

Budget::~Budget()
{

}

bool Budget::read(const QJsonObject &json)
{
    /**/
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name = json[KW_NAME].toString();
    _amount = json[KW_AMOUNT].toDouble();
    _description = json[KW_DESCRIPTION].toString();
    /**/
    _valid = true;
end:
    return _valid;
}

bool Budget::write(QJsonObject &json) const
{
    /**/
    json[KW_NAME] = _name;
    json[KW_AMOUNT] = _amount;
    json[KW_DESCRIPTION] = _description;
    /**/
    return true;
}

