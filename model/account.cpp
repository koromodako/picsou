#include "account.h"

#include <QJsonArray>

Account::Account() :
    PicsouModelObj(false)
{

}

Account::~Account()
{

}

bool Account::remove_payment_method(QUuid id)
{
    switch (_payment_methods.remove(id)) {
        case 0: /* TRACE */ return false;
        case 1: return true;
        default: /* TRACE */ return false;
    }
}

bool Account::remove_scheduled_operation(QUuid id)
{
    switch (_scheduled_ops.remove(id)) {
        case 0: /* TRACE */ return false;
        case 1: return true;
        default: /* TRACE */ return false;
    }
}

bool Account::remove_operation(QUuid id)
{
    switch (_ops.remove(id)) {
        case 0: /* TRACE */ return false;
        case 1: return true;
        default: /* TRACE */ return false;
    }
}

bool Account::read(const QJsonObject &json)
{
    QJsonArray array;
    if(!json.contains("payment_methods") ||
       !json.contains("scheduled_ops") ||
       !json.contains("ops")) {
        _valid = false;
        goto end;
    }
    JSON_READ_LIST(json, "payment_methods", _payment_methods, PaymentMethod);
    JSON_READ_LIST(json, "scheduled_ops", _scheduled_ops, ScheduledOperation);
    JSON_READ_LIST(json, "ops", _ops, Operation);
    _valid = true;
end:
    return _valid;
}

bool Account::write(QJsonObject &json) const
{
    bool ok;
    QJsonArray array;
    QJsonObject obj;
    /**/
    JSON_WRITE_LIST(json, "payment_methods", _payment_methods.values());
    JSON_WRITE_LIST(json, "scheduled_ops", _scheduled_ops.values());
    JSON_WRITE_LIST(json, "ops", _ops.values());
    /**/
    ok = true;
end:
    return ok;
}
