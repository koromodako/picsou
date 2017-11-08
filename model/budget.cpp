#include "budget.h"

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
    if(!json.contains("name") ||
       !json.contains("amount") ||
       !json.contains("description")) {
        /* TRACE */
        _valid = false;
        goto end;
    }
    _name = json["name"].toString();
    _amount = json["amount"].toDouble();
    _description = json["description"].toString();
    /**/
    _valid = true;
end:
    return _valid;
}

bool Budget::write(QJsonObject &json) const
{
    /**/
    json["name"] = _name;
    json["amount"] = _amount;
    json["description"] = _description;
    /**/
    return true;
}

