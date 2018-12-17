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
#ifndef OPERATIONVIEWER_H
#define OPERATIONVIEWER_H

#include "ui/picsouuiviewer.h"
#include "ui/widgets/picsoutablewidget.h"
#include "ui/widgets/operationstatistics.h"

namespace Ui {
class OperationViewer;
}

class OperationViewer : public PicsouUIViewer
{
    Q_OBJECT
public:
    enum ViewerScale {
        VS_YEAR,
        VS_MONTH
    };

    virtual ~OperationViewer();
    explicit OperationViewer(PicsouUIService *ui_svc,
                             QUuid user_id,
                             QUuid account_id,
                             ViewerScale scale,
                             int year=-1,
                             int month=-1,
                             QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    /* ops */
    void add_op();
    void edit_op();
    void remove_op();
    void table_edit_op(int row, int col);

private:
    QUuid m_user_id;
    int m_year;
    int m_month;
    ViewerScale m_scale;
    PicsouTableWidget *m_table;
    OperationStatistics *m_ops_stats;
    Ui::OperationViewer *ui;
};

#endif // OPERATIONVIEWER_H
