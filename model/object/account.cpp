#include "account.h"
#include "utils/macro.h"

#include <QSet>
#include <QJsonArray>

#define KEYS \
        (QStringList() << KW_ACT_NAME \
                       << KW_ACT_NOTES \
                       << KW_ACT_PAYMENT_METHODS \
                       << KW_ACT_SCHEDULED_OPS \
                       << KW_ACT_OPS)

Account::~Account()
{
    DELETE_HASH_CONTENT(ScheduledOperationPtr, _scheduled_ops);
    DELETE_HASH_CONTENT(PaymentMethodPtr, _payment_methods);
    DELETE_HASH_CONTENT(OperationPtr, _ops);
}

Account::Account(PicsouModelObj *parent) :
    PicsouModelObj(false, parent),
    _name(QString()),
    _notes(QString())
{

}

Account::Account(const QString &name,
                 const QString &notes,
                 PicsouModelObj *parent) :
    PicsouModelObj(true, parent),
    _name(name),
    _notes(notes)
{

}

void Account::update(const QString &name, const QString &notes)
{
    _name=name;
    _notes=notes;
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

void Account::add_operation(Amount amount,
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

void Account::add_operations(OperationCollection ops)
{
    foreach (OperationPtr op, ops.list()) {
        op->set_parent(this);
        _ops.insert(op->id(), op);
    }
    if(ops.length()>0) {
        emit modified();
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
    OperationCollection operations=ops();
    QSet<int> years_set;
    QList<int> years_list;
    foreach (OperationPtr op, operations.list()) {
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

bool op_cmp(const OperationPtr &a, const OperationPtr &b)
{
    return a->date() < b->date();
}

OperationCollection Account::ops(bool sorted) const
{
    OperationPtrList ops=_ops.values();
    if(sorted) {
        std::sort(ops.begin(), ops.end(), op_cmp);
    }
    return ops;
}

bool pm_cmp(const PaymentMethodPtr &a, const PaymentMethodPtr &b)
{
    return a->name() < b->name();
}

PaymentMethodPtrList Account::payment_methods(bool sorted) const
{
    PaymentMethodPtrList p_methods=_payment_methods.values();
    if(sorted) {
        std::sort(p_methods.begin(), p_methods.end(), pm_cmp);
    }
    return p_methods;
}

QStringList Account::payment_methods_str(bool sorted) const
{
    QStringList p_methods_str;
    PaymentMethodPtrList p_methods=payment_methods(sorted);
    foreach (PaymentMethodPtr pm, p_methods) {
        p_methods_str << pm->name();
    }
    return p_methods_str;
}

bool Account::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>")
    JSON_CHECK_KEYS(KEYS);
    /**/
    _name=json[KW_ACT_NAME].toString();
    _notes=json[KW_ACT_NOTES].toString();
    JSON_READ_LIST(json, KW_ACT_PAYMENT_METHODS,
                   _payment_methods, PaymentMethod, this);
    JSON_READ_LIST(json, KW_ACT_SCHEDULED_OPS,
                   _scheduled_ops, ScheduledOperation, this);
    JSON_READ_LIST(json, KW_ACT_OPS,
                   _ops, Operation, this);
    /**/
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool Account::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_ACT_NAME]=_name;
    json[KW_ACT_NOTES]=_notes;
    JSON_WRITE_LIST(json, KW_ACT_PAYMENT_METHODS, _payment_methods.values());
    JSON_WRITE_LIST(json, KW_ACT_SCHEDULED_OPS, _scheduled_ops.values());
    JSON_WRITE_LIST(json, KW_ACT_OPS, _ops.values());
    /**/
    LOG_BOOL_RETURN(true);
}

bool Account::operator <(const Account &other)
{
    return (_name < other._name);
}
