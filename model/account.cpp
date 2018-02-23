#include "account.h"

#include <QSet>
#include <QJsonArray>

#define KW_NAME "name"
#define KW_PAYMENT_METHODS "payment_methods"
#define KW_SCHEDULED_OPS "scheduled_ops"
#define KW_OPS "ops"
#define KEYS \
        (QStringList() << KW_NAME << KW_PAYMENT_METHODS \
         << KW_SCHEDULED_OPS << KW_OPS)

Account::~Account()
{
    DELETE_HASH_CONTENT(ScheduledOperationPtr, _scheduled_ops);
    DELETE_HASH_CONTENT(PaymentMethodPtr, _payment_methods);
    DELETE_HASH_CONTENT(OperationPtr, _ops);
}

Account::Account(PicsouModelObj *parent) :
    PicsouModelObj(false, parent),
    _name(QString()),
    _description(QString())
{

}

Account::Account(QString name,
                 QString description,
                 PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
    _name(name),
    _description(description)
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

QList<int> Account::years(bool sorted) const
{
    QList<OperationPtr> operations=ops();
    QSet<int> years_set;
    QList<int> years_list;
    foreach (OperationPtr op, operations) {
        years_set << op->date().year();
    }
    years_list=years_set.toList();
    if(sorted) {
        std::sort(years_list.begin(), years_list.end());
    }
    return years_list;
}

QList<OperationPtr> Account::ops(bool sorted) const
{
    QList<OperationPtr> ops=_ops.values();
    if(sorted) {
        std::sort(ops.begin(), ops.end());
    }
    return ops;
}

QList<PaymentMethodPtr> Account::payment_methods(bool sorted) const
{
    QList<PaymentMethodPtr> p_methods=_payment_methods.values();
    if(sorted) {
        std::sort(p_methods.begin(), p_methods.end());
    }
    return p_methods;
}

bool Account::read(const QJsonObject &json)
{
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name = json[KW_NAME].toString();
    JSON_READ_LIST(json, "payment_methods",
                   _payment_methods, PaymentMethod, this);
    JSON_READ_LIST(json, "scheduled_ops",
                   _scheduled_ops, ScheduledOperation, this);
    JSON_READ_LIST(json, "ops",
                   _ops, Operation, this);
    /**/
    set_valid();
end:
    return valid();
}

bool Account::write(QJsonObject &json) const
{
    bool ok;
    /**/
    json[KW_NAME] = _name;
    JSON_WRITE_LIST(json, "payment_methods", _payment_methods.values());
    JSON_WRITE_LIST(json, "scheduled_ops", _scheduled_ops.values());
    JSON_WRITE_LIST(json, "ops", _ops.values());
    /**/
    ok = true;
end:
    return ok;
}

bool Account::operator <(const Account &other)
{
    return (_name < other._name);
}
