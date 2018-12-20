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
#ifndef OPERATION_H
#define OPERATION_H

#include "utils/amount.h"
#include "model/picsoudbo.h"

#include <QDate>

class PaymentMethod;
DECL_PICSOU_MOD_OBJ_PTR(PaymentMethod,
                        PaymentMethodPtr,
                        PaymentMethodShPtr,
                        PaymentMethodPtrList,
                        PaymentMethodShPtrList);

class Operation : public PicsouDBO
{
    Q_OBJECT
public:
    enum Type {
        NEUTRAL,
        CREDIT,
        DEBIT
    };

    static const QString KW_AMOUNT;
    static const QString KW_DAY;
    static const QString KW_MONTH;
    static const QString KW_YEAR;
    static const QString KW_BUDGET;
    static const QString KW_RECIPIENT;
    static const QString KW_DESCRIPTION;
    static const QString KW_PAYMENT_METHOD;

    Operation(PicsouDBO *parent);
    Operation(const Amount &amount,
              const QDate &date,
              const QString &budget,
              const QString &recipient,
              const QString &description,
              const QString &payment_method,
              PicsouDBO *parent);

    void update(Amount amount,
                const QDate &date,
                const QString &budget,
                const QString &recipient,
                const QString &description,
                const QString &payment_method);

    void mark_scheduled() { m_scheduled=true; }

    inline Amount amount() const { return m_amount; }
    inline QDate date() const { return m_date; }
    inline QString budget() const { return m_budget; }
    inline QString recipient() const { return m_recipient; }
    inline QString description() const { return m_description; }
    inline QString payment_method() const { return m_payment_method; }
    inline Type type() const { return (m_amount==0.?NEUTRAL:(m_amount<0.?DEBIT:CREDIT)); }
    inline bool scheduled() const { return m_scheduled; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    inline bool operator<(const Operation &other) { return m_date<other.m_date; }

private:
    Amount m_amount;
    QDate m_date;
    QString m_budget;
    QString m_recipient;
    QString m_description;
    QString m_payment_method;
    bool m_scheduled=false;
};

DECL_PICSOU_MOD_OBJ_PTR(Operation,
                        OperationPtr,
                        OperationShPtr,
                        OperationPtrList,
                        OperationShPtrList);

#endif // OPERATION_H
