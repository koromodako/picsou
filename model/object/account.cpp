/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "account.h"
#include "utils/macro.h"

#include <QSet>
#include <QJsonArray>

const QString Account::KW_OPS="ops";
const QString Account::KW_NAME="name";
const QString Account::KW_NOTES="notes";
const QString Account::KW_PAYMENT_METHODS="payment_methods";
const QString Account::KW_SCHEDULED_OPS="scheduled_ops";

Account::~Account()
{
    DELETE_HASH_CONTENT(ScheduledOperationPtr, m_scheduled_ops);
    DELETE_HASH_CONTENT(PaymentMethodPtr, m_payment_methods);
    DELETE_HASH_CONTENT(OperationPtr, m_ops);
}

Account::Account(PicsouDBO *parent) :
    PicsouDBO(false, parent),
    m_name(QString()),
    m_notes(QString())
{

}

Account::Account(const QString &name,
                 const QString &notes,
                 PicsouDBO *parent) :
    PicsouDBO(true, parent),
    m_name(name),
    m_notes(notes)
{

}

void Account::update(const QString &name, const QString &notes)
{
    m_name=name;
    m_notes=notes;
    emit modified();
}

void Account::add_payment_method(const QString &name)
{
    PaymentMethodPtr pm=PaymentMethodPtr(new PaymentMethod(name, this));
    m_payment_methods.insert(pm->id(), pm);
    emit modified();
}

bool Account::remove_payment_method(QUuid id)
{
    bool success=false;
    switch (m_payment_methods.remove(id)) {
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

void Account::add_scheduled_operation(const Amount &amount,
                                      const QString &budget,
                                      const QString &recipient,
                                      const QString &description,
                                      const QString &payment_method,
                                      const QString &name,
                                      const Schedule &schedule)
{
    ScheduledOperationPtr sop=ScheduledOperationPtr(new ScheduledOperation(amount,
                                                                           budget,
                                                                           recipient,
                                                                           description,
                                                                           payment_method,
                                                                           name,
                                                                           schedule,
                                                                           this));
    m_scheduled_ops.insert(sop->id(), sop);
    emit modified();
}

bool Account::remove_scheduled_operation(QUuid id)
{
    bool success=false;
    switch (m_scheduled_ops.remove(id)) {
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

void Account::add_operation(const Amount &amount,
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
     m_ops.insert(op->id(), op);
     emit modified();
}

void Account::add_operations(const OperationPtrList &ops)
{
    for(const auto &op : ops) {
        op->set_parent(this);
        m_ops.insert(op->id(), op);
    }
    if(ops.length()>0) {
        emit modified();
    }
}

bool Account::remove_operation(QUuid id)
{
    bool success=false;
    switch (m_ops.remove(id)) {
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
    QHash<QUuid, PaymentMethodPtr>::const_iterator it=m_payment_methods.find(id);
    if(it!=m_payment_methods.end()) {
        pm=*it;
    }
    return pm;
}

ScheduledOperationPtr Account::find_scheduled_operation(QUuid id)
{
    ScheduledOperationPtr sop;
    QHash<QUuid, ScheduledOperationPtr>::const_iterator it=m_scheduled_ops.find(id);
    if(it!=m_scheduled_ops.end()) {
        sop=*it;
    }
    return sop;
}

OperationPtr Account::find_operation(QUuid id)
{
    OperationPtr op;
    QHash<QUuid, OperationPtr>::const_iterator it=m_ops.find(id);
    if(it!=m_ops.end()) {
        op=*it;
    }
    return op;
}

#define min(a, b) (((a)<(b))?(a):(b))

int Account::min_year() const
{
    int min_y=INT_MAX;
    for(const auto &op : m_ops) {
        min_y=min(min_y, op->date().year());
    }
    for(const auto &sop : m_scheduled_ops) {
        min_y=min(min_y, sop->schedule().from().year());
    }
    return min_y;

}

#undef min

bool pm_cmp(const PaymentMethodPtr &a, const PaymentMethodPtr &b)
{
    return a->name()<b->name();
}

PaymentMethodPtrList Account::payment_methods(bool sorted) const
{
    PaymentMethodPtrList p_methods=m_payment_methods.values();
    if(sorted) {
        std::sort(p_methods.begin(), p_methods.end(), pm_cmp);
    }
    return p_methods;
}

QStringList Account::payment_methods_str(bool sorted) const
{
    QStringList p_methods_str;
    PaymentMethodPtrList p_methods=payment_methods(sorted);
    for(auto &pm : p_methods) {
        p_methods_str<<pm->name();
    }
    return p_methods_str;
}

bool Account::read(const QJsonObject &json)
{
    LOG_IN("<QJsonObject>")
    static const QStringList keys=(QStringList()<<KW_NAME
                                                <<KW_NOTES
                                                <<KW_PAYMENT_METHODS
                                                <<KW_SCHEDULED_OPS
                                                <<KW_OPS);
    JSON_CHECK_KEYS(keys);
    /**/
    m_name=json[KW_NAME].toString();
    m_notes=json[KW_NOTES].toString();
    JSON_READ_LIST(json, KW_PAYMENT_METHODS,
                   m_payment_methods, PaymentMethod, this);
    JSON_READ_LIST(json, KW_SCHEDULED_OPS,
                   m_scheduled_ops, ScheduledOperation, this);
    JSON_READ_LIST(json, KW_OPS,
                   m_ops, Operation, this);
    /**/
    set_valid();
    LOG_BOOL_RETURN(valid());
}

bool Account::write(QJsonObject &json) const
{
    LOG_IN("<QJsonObject>")
    json[KW_NAME]=m_name;
    json[KW_NOTES]=m_notes;
    JSON_WRITE_LIST(json, KW_PAYMENT_METHODS, m_payment_methods.values());
    JSON_WRITE_LIST(json, KW_SCHEDULED_OPS, m_scheduled_ops.values());
    JSON_WRITE_LIST(json, KW_OPS, m_ops.values());
    /**/
    LOG_BOOL_RETURN(true);
}

bool Account::operator <(const Account &other)
{
    return (m_name<other.m_name);
}
