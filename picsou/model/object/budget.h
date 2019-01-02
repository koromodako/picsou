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
#ifndef BUDGET_H
#define BUDGET_H

#include "utils/macro.h"
#include "utils/amount.h"
#include "model/picsoudbo.h"

class Budget : public PicsouDBO
{
    Q_OBJECT
public:
    static const QString KW_NAME;
    static const QString KW_AMOUNT;
    static const QString KW_DESCRIPTION;

    Budget(PicsouDBO *parent);
    Budget(const Amount &amount,
           const QString &name,
           const QString &description,
           PicsouDBO *parent);

    void update(const Amount &amount,
                const QString &name,
                const QString &description);

    inline Amount amount() const { return m_amount; }
    inline QString name() const { return m_name; }
    inline QString description() const { return m_description; }

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    bool operator <(const Budget &other);

private:
    Amount m_amount;
    QString m_name;
    QString m_description;
};

DECL_PICSOU_OBJ_PTR(Budget, BudgetShPtr, BudgetShPtrList);

#endif // BUDGET_H
