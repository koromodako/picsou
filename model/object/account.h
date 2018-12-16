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
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "paymentmethod.h"
#include "scheduledoperation.h"

#include <QHash>


class Account : public PicsouModelObj
{
    Q_OBJECT
public:
    static const QString KW_OPS;
    static const QString KW_NAME;
    static const QString KW_NOTES;
    static const QString KW_PAYMENT_METHODS;
    static const QString KW_SCHEDULED_OPS;

    virtual ~Account();
    Account(PicsouModelObj *parent);
    Account(const QString &name,
            const QString &notes,
            PicsouModelObj *parent);

    void update(const QString &name,
                const QString &notes);

    void add_payment_method(const QString &name);
    bool remove_payment_method(QUuid id);

    void add_scheduled_operation(const Amount &amount,
                                 const QString &budget,
                                 const QString &recipient,
                                 const QString &description,
                                 const QString &payment_method,
                                 const QString &name,
                                 const Schedule &schedule);
    bool remove_scheduled_operation(QUuid id);

    void add_operation(const Amount &amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method);
    void add_operations(const OperationPtrList &ops);
    bool remove_operation(QUuid id);

    PaymentMethodPtr find_payment_method(QUuid id);
    ScheduledOperationPtr find_scheduled_operation(QUuid id);
    OperationPtr find_operation(QUuid id);

    inline QString name() const { return _name; }
    inline QString notes() const { return _notes; }
    inline ScheduledOperationPtrList scheduled_ops() const { return _scheduled_ops.values(); }
    inline OperationPtrList ops() const { return _ops.values(); }

    int min_year() const;
    QStringList payment_methods_str(bool sorted=false) const;
    PaymentMethodPtrList payment_methods(bool sorted=false) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const Account &other);

private:
    QString _name;
    QString _notes;
    QHash<QUuid, PaymentMethodPtr> _payment_methods;
    QHash<QUuid, ScheduledOperationPtr> _scheduled_ops;
    QHash<QUuid, OperationPtr> _ops;
};

DECL_PICSOU_MOD_OBJ_PTR(Account,
                        AccountPtr,
                        AccountShPtr,
                        AccountPtrList,
                        AccountShPtrList);

#endif // ACCOUNT_H
