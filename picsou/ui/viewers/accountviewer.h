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
#ifndef ACCOUNTVIEWER_H
#define ACCOUNTVIEWER_H

#include "ui/picsouuiviewer.h"
#include "ui/widgets/picsoutablewidget.h"
#include "ui/widgets/operationstatistics.h"

namespace Ui {
class AccountViewer;
}

class AccountViewer : public PicsouUIViewer
{
    Q_OBJECT

public:
    virtual ~AccountViewer();
    explicit AccountViewer(PicsouUIServicePtr ui_svc,
                           QUuid user_uuid,
                           QUuid account_uuid,
                           QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    /* payment methods */
    void add_pm();
    void edit_pm();
    void remove_pm();
    /* scheduled ops */
    void add_sop();
    void edit_sop();
    void remove_sop();
    /* ops */
    void add_op();
    void edit_op();
    void remove_op();
    void import_ops();
    void export_ops();

    void table_edit_op(int row, int col);

private:
    QUuid m_user_id;
    QString m_rolling_expense_lab;
    PicsouTableWidget *m_table;
    OperationStatistics *m_ops_stats;
    Ui::AccountViewer *ui;
};

#endif // ACCOUNTVIEWER_H
