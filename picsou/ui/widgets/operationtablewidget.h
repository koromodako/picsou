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
#ifndef OPERATIONTABLEWIDGET_H
#define OPERATIONTABLEWIDGET_H

#include <QTableWidget>

#include "model/operationcollection.h"

class OperationTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    OperationTableWidget(QWidget *parent=nullptr);

    void clear();
    void refresh(OperationCollection ops);
    bool is_current_op_scheduled() const;
    QUuid current_op(QTableWidgetItem *item=nullptr) const;

    void set_readonly(bool ro) { m_readonly=ro; }

signals:
    void op_edit_requested(int row, int col);
    void op_verified_state_changed(QUuid id, bool checked);

public slots:
    void verified_checked(QTableWidgetItem *item);

private:
    Q_DISABLE_COPY(OperationTableWidget)
    bool m_readonly=false;
};

#endif // OPERATIONTABLEWIDGET_H
