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

Account::Account(const QString &name,
                 const QString &description,
                 PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
    _name(name),
    _description(description)
{

}

void Account::update(const QString &name, const QString &description)
{
    _name=name;
    _description=description;
    emit modified();
}

void Account::add_payment_method(const QString &name)
{
    PaymentMethodPtr pm=PaymentMethodPtr(new PaymentMethod(name, this));
    _payment_methods.insert(pm->id(), pm);
    emit modified();
}

bool Account::remove_payment_method(QUuid id)
{
    bool success=false;
    switch (_payment_methods.remove(id)) {
    case 0:
        /* TRACE */
        break;
    case 1:
        success=true;
        emit modified();
        break;
    default:
        /* TRACE */
        break;
    }
    return success;
}

bool Account::remove_scheduled_operation(QUuid id)
{
    bool success=false;
    switch (_scheduled_ops.remove(id)) {
    case 0:
        /* TRACE */
        break;
    case 1:
        success=true;
        emit modified();
        break;
    default:
        /* TRACE */
        break;
    }
    return success;
}

void Account::add_operation(double amount,
                            const QDate &date,
                            const QString &budget,
                            const QString &recipient,
                            const QString &description,
                            const QString &payment_method)
{
    OperationPtr op=OperationPtr(new Operation(amount,
                                               date,
                                               budget,
                                               recipient,
                                               description,
                                               payment_method,
                                               this));
     _ops.insert(op->id(), op);
     emit modified();
}

void Account::add_operations(QList<OperationPtr> ops)
{
    foreach (OperationPtr op, ops) {
        op->set_parent(this);
        _ops.insert(op->id(), op);
    }
}

bool Account::remove_operation(QUuid id)
{
    bool success=false;
    switch (_ops.remove(id)) {
    case 0:
        /* TRACE */
        break;
    case 1:
        success=true;
        emit modified();
        break;
    default:
        /* TRACE */
        break;
    }
    return success;
}

PaymentMethodPtr Account::find_payment_method(QUuid id)
{
    PaymentMethodPtr pm;
    QHash<QUuid, PaymentMethodPtr>::const_iterator it=_payment_methods.find(id);
    if(it!=_payment_methods.end()) {
        pm=*it;
    }
    return pm;
}

ScheduledOperationPtr Account::find_scheduled_operation(QUuid id)
{
    ScheduledOperationPtr sop;
    QHash<QUuid, ScheduledOperationPtr>::const_iterator it=_scheduled_ops.find(id);
    if(it!=_scheduled_ops.end()) {
        sop=*it;
    }
    return sop;
}

OperationPtr Account::find_operation(QUuid id)
{
    OperationPtr op;
    QHash<QUuid, OperationPtr>::const_iterator it=_ops.find(id);
    if(it!=_ops.end()) {
        op=*it;
    }
    return op;
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
    if(years_list.empty()) {
        years_list << QDate::currentDate().year();
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

QStringList Account::payment_methods_str(bool sorted) const
{
    QStringList p_methods_str;
    QList<PaymentMethodPtr> p_methods=payment_methods(sorted);
    foreach (PaymentMethodPtr pm, p_methods) {
        p_methods_str << pm->name();
    }
    return p_methods_str;
}

bool Account::read(const QJsonObject &json)
{
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name=json[KW_NAME].toString();
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
    json[KW_NAME]=_name;
    JSON_WRITE_LIST(json, "payment_methods", _payment_methods.values());
    JSON_WRITE_LIST(json, "scheduled_ops", _scheduled_ops.values());
    JSON_WRITE_LIST(json, "ops", _ops.values());
    /**/
    ok=true;
end:
    return ok;
}

bool Account::operator <(const Account &other)
{
    return (_name < other._name);
}
