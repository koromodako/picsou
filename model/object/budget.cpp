#include "budget.h"
#include "utils/macro.h"

const QString Budget::KW_NAME="name";
const QString Budget::KW_AMOUNT="amount";
const QString Budget::KW_DESCRIPTION="description";

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

bool Budget::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>")
    static const QStringList keys=(QStringList()<<Budget::KW_NAME
                                                <<Budget::KW_AMOUNT
                                                <<Budget::KW_DESCRIPTION);
    JSON_CHECK_KEYS(keys);
    /**/
    _name=json[KW_NAME].toString();
    _amount=json[KW_AMOUNT].toDouble();
    _description=json[KW_DESCRIPTION].toString();
    /**/
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool Budget::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_NAME]=_name;
    json[KW_AMOUNT]=_amount;
    json[KW_DESCRIPTION]=_description;
    /**/
    LOG_BOOL_RETURN(true);
}

bool Budget::operator <(const Budget &other)
{
    return (_name < other._name);
}

