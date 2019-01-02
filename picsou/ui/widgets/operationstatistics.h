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
#ifndef OPERATIONSTATISTICS_H
#define OPERATIONSTATISTICS_H

#include <QWidget>

#include "model/object/budget.h"
#include "model/operationcollection.h"

class QLabel; /* predecl */

namespace Ui {
class OperationStatistics;
}

class OperationStatistics : public QWidget
{
    Q_OBJECT

public:
    virtual ~OperationStatistics();
    explicit OperationStatistics(QWidget *parent=nullptr);

    void clear();
    void refresh(const OperationCollection &ops, const BudgetShPtrList &user_budgets);

    bool append_field(const QString &name, const QString &value);
    bool update_field(const QString &name, const QString &value);
    bool remove_field(const QString &name);

private:
    QHash<QString, QLabel*> m_extra_fields;
    Ui::OperationStatistics *ui;
};

#endif // OPERATIONSTATISTICS_H
