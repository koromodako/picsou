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


class Account : public PicsouDBO
{
    Q_OBJECT
public:
    static const QString KW_OPS;
    static const QString KW_NAME;
    static const QString KW_NOTES;
    static const QString KW_ARCHIVED;
    static const QString KW_INITIAL_AMOUNT;
    static const QString KW_PAYMENT_METHODS;
    static const QString KW_SCHEDULED_OPS;

    Account(PicsouDBO *parent);
    Account(const QString &name,
            const QString &notes,
            bool archived,
            const Amount &intial_amount,
            PicsouDBO *parent);

    void update(const QString &name,
                const QString &notes,
                bool archived,
                const Amount &initial_amount);

    bool add_payment_method(const QString &name, QString &error);
    bool remove_payment_method(QUuid id);

    bool add_scheduled_operation(const Amount &amount,
                                 const QString &budget,
                                 const QString &recipient,
                                 const QString &description,
                                 const QString &payment_method,
                                 const QString &name,
                                 const Schedule &schedule,
                                 QString &error);
    bool remove_scheduled_operation(QUuid id, QString &error);

    bool add_operation(bool verified,
                       const Amount &amount,
                       const QDate &date,
                       const QString &budget,
                       const QString &recipient,
                       const QString &description,
                       const QString &payment_method,
                       QString &error);
    bool add_operations(const OperationShPtrList &ops, QString &error);
    bool remove_operation(QUuid id, QString &error);

    PaymentMethodShPtr find_payment_method(QUuid id);
    PaymentMethodShPtr find_payment_method(const QString &name);
    ScheduledOperationShPtr find_scheduled_operation(QUuid id);
    OperationShPtr find_operation(QUuid id);


    inline bool archived() const { return m_archived; }
    inline QString name() const { return m_name; }
    inline QString notes() const { return m_notes; }
    inline Amount initial_amount() const { return m_initial_amount; }
    inline ScheduledOperationShPtrList scheduled_ops() const { return m_scheduled_ops.values(); }
    inline OperationShPtrList ops() const { return m_ops.values(); }

    int min_year() const;
    QStringList srcdst() const;
    QStringList payment_methods_str(bool sorted=false) const;
    PaymentMethodShPtrList payment_methods(bool sorted=false) const;

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const Account &other);

private:
    QString m_name;
    QString m_notes;
    bool m_archived;
    Amount m_initial_amount;
    QHash<QUuid, PaymentMethodShPtr> m_payment_methods;
    QHash<QUuid, ScheduledOperationShPtr> m_scheduled_ops;
    QHash<QUuid, OperationShPtr> m_ops;
};

DECL_PICSOU_OBJ_PTR(Account, AccountShPtr, AccountShPtrList);

#endif // ACCOUNT_H
