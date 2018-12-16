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
#include "operationviewer.h"
#include "ui_operationviewer.h"

#include "app/picsouuiservice.h"
#include "ui/items/picsoutableitem.h"

OperationViewer::~OperationViewer()
{
    delete _ops_stats;
    delete _table;
    delete ui;
}

OperationViewer::OperationViewer(PicsouUIService *ui_svc,
                                 QUuid user_id,
                                 QUuid account_id,
                                 ViewerScale scale,
                                 int year,
                                 int month,
                                 QWidget *parent) :
    PicsouUIViewer(ui_svc, account_id, parent),
    _user_id(user_id),
    _year(year),
    _month(month),
    _scale(scale),
    ui(new Ui::OperationViewer)
{
    ui->setupUi(this);
    connect(ui_svc, &PicsouUIService::model_updated,
            this, &OperationViewer::refresh);

    _table=new PicsouTableWidget;
    ui->main_layout->insertWidget(0, _table);

    _ops_stats=new OperationStatistics;
    ui->main_layout->addWidget(_ops_stats);

    /* ops */
    connect(ui->add_op, &QPushButton::clicked,
            this, &OperationViewer::add_op);
    connect(ui->action_add_op, &QAction::triggered,
            this, &OperationViewer::add_op);
    addAction(ui->action_add_op);

    connect(ui->edit_op, &QPushButton::clicked,
            this, &OperationViewer::edit_op);
    connect(ui->action_edit_op, &QAction::triggered,
            this, &OperationViewer::edit_op);
    connect(_table, &PicsouTableWidget::cellDoubleClicked,
            this, &OperationViewer::table_edit_op);
    addAction(ui->action_edit_op);

    connect(ui->remove_op, &QPushButton::clicked,
            this, &OperationViewer::remove_op);
    connect(ui->action_remove_op, &QAction::triggered,
            this, &OperationViewer::remove_op);
    addAction(ui->action_remove_op);
}

void OperationViewer::refresh(const PicsouDBPtr db)
{
    int year=-1, month=-1;
    OperationCollection ops;

    switch (_scale) {
        case VS_YEAR:
            year=_year;
            break;
        case VS_MONTH:
            year=_year;
            month=_month;
            break;
    }

    ops=db->ops(mod_obj_id(), year, month);
    _table->refresh(ops);
    _ops_stats->refresh(ops);
}

void OperationViewer::add_op()
{
    ui_svc()->op_add(_user_id, mod_obj_id(), _year, _month);
}

void OperationViewer::edit_op()
{
    QUuid op_id;
    op_id=_table->current_op();
    if(!op_id.isNull()) {
        ui_svc()->op_edit(_user_id, mod_obj_id(), op_id, _year, _month);
    }
}

void OperationViewer::remove_op()
{
    QUuid op_id;
    op_id=_table->current_op();
    if(!op_id.isNull()) {
        ui_svc()->op_remove(mod_obj_id(), op_id);
    }
}

void OperationViewer::table_edit_op(int, int)
{
    edit_op();
}
