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
    delete m_ops_stats;
    delete m_table;
    delete ui;
}

OperationViewer::OperationViewer(PicsouUIServicePtr ui_svc,
                                 QUuid user_id,
                                 QUuid account_id,
                                 bool readonly,
                                 TimeScale scale,
                                 int year,
                                 int month,
                                 QWidget *parent) :
    PicsouUIViewer(ui_svc, account_id, parent),
    m_year(year),
    m_month(month),
    m_readonly(readonly),
    m_user_id(user_id),
    m_scale(scale),
    ui(new Ui::OperationViewer)
{
    ui->setupUi(this);
    connect(ui_svc, &PicsouUIService::notify_model_updated, this, &OperationViewer::refresh);

    m_table=new OperationTableWidget;
    ui->main_layout->insertWidget(0, m_table);

    m_ops_stats=new OperationStatistics;
    ui->main_layout->addWidget(m_ops_stats);

    /* ops */
    connect(ui->op_add, &QPushButton::clicked, this, &OperationViewer::add_op);
    connect(ui->action_add_op, &QAction::triggered, this, &OperationViewer::add_op);
    addAction(ui->action_add_op);

    connect(ui->op_edit, &QPushButton::clicked, this, &OperationViewer::edit_op);
    connect(ui->action_edit_op, &QAction::triggered, this, &OperationViewer::edit_op);
    connect(m_table, &OperationTableWidget::op_edit_requested, this, &OperationViewer::table_edit_op);
    connect(m_table, &OperationTableWidget::op_verified_state_changed, this, &OperationViewer::table_update_op_verified);
    addAction(ui->action_edit_op);

    connect(ui->op_remove, &QPushButton::clicked, this, &OperationViewer::remove_op);
    connect(ui->action_remove_op, &QAction::triggered, this, &OperationViewer::remove_op);
    addAction(ui->action_remove_op);
}

void OperationViewer::refresh(const PicsouDBShPtr db)
{
    int year=-1, month=-1;
    OperationCollection ops;

    switch (m_scale) {
        case VS_YEAR:
            year=m_year;
            break;
        case VS_MONTH:
            year=m_year;
            month=m_month;
            break;
    }

    ops=db->ops(mod_obj_id(), year, month);
    m_table->set_readonly(m_readonly);
    m_table->refresh(ops);

    UserShPtr user=db->find_user(m_user_id);
    if(user.isNull()) {
        LOG_WARNING("invalid user pointer");
        return;
    }
    m_ops_stats->refresh(ops, user->budgets());

    bool has_ops=ops.length()>0;
    ui->op_add->setEnabled(!m_readonly);
    ui->op_edit->setEnabled(has_ops&&!m_readonly);
    ui->op_remove->setEnabled(has_ops&&!m_readonly);
}

void OperationViewer::add_op()
{
    ui_svc()->op_add(m_user_id, mod_obj_id(), m_year, m_month);
}

void OperationViewer::edit_op()
{
    QUuid op_id;
    if(m_table->is_current_op_scheduled()) {
        ui_svc()->svc_op_failed(tr("Logical error: you can't edit a scheduled operation from this view."));
        return;
    }
    op_id=m_table->current_op();
    if(!op_id.isNull()) {
        ui_svc()->op_edit(m_user_id, mod_obj_id(), op_id, m_year, m_month);
    }
}

void OperationViewer::remove_op()
{
    QUuid op_id;
    if(m_table->is_current_op_scheduled()) {
        ui_svc()->svc_op_failed(tr("Logical error: you can't remove a scheduled operation from this view."));
        return;
    }
    op_id=m_table->current_op();
    if(!op_id.isNull()) {
        ui_svc()->op_remove(mod_obj_id(), op_id);
    }
}

void OperationViewer::table_edit_op(int, int)
{
    edit_op();
}

void OperationViewer::table_update_op_verified(QUuid op_id, bool verified)
{
    if(!op_id.isNull()) {
        ui_svc()->op_set_verified(mod_obj_id(), op_id, verified);
    }
}
