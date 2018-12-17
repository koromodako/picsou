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
#ifndef PAYMENTMETHOD_H
#define PAYMENTMETHOD_H

#include "../picsoudbo.h"


class PaymentMethod : public PicsouDBO
{
    Q_OBJECT
public:
    static const QString KW_NAME;

    PaymentMethod(PicsouDBO *parent);
    PaymentMethod(const QString &name,
                  PicsouDBO *parent);

    void update(const QString &name);

    inline QString name() const { return m_name; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const PaymentMethod &other);

private:
    QString m_name;
};

DECL_PICSOU_MOD_OBJ_PTR(PaymentMethod,
                        PaymentMethodPtr,
                        PaymentMethodShPtr,
                        PaymentMethodPtrList,
                        PaymentMethodShPtrList);

#endif // PAYMENTMETHOD_H
