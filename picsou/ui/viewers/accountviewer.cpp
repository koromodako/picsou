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
#include "accountviewer.h"
#include "ui_accountviewer.h"

#include "utils/macro.h"

#include "app/picsouuiservice.h"
#include "ui/items/picsoulistitem.h"
#include "ui/viewers/operationviewer.h"

AccountViewer::~AccountViewer()
{
    delete m_ops_stats;
    delete m_table;
    delete ui;
}

AccountViewer::AccountViewer(PicsouUIServicePtr ui_svc,
                             QUuid user_uuid,
                             QUuid account_uuid,
                             QWidget *parent) :
    PicsouUIViewer(ui_svc, account_uuid, parent),
    m_user_id(user_uuid),
    m_rolling_expense_lab(tr("Rolling expense (30 days):")),
    ui(new Ui::AccountViewer)
{
    ui->setupUi(this);
    connect(ui_svc, &PicsouUIService::notify_model_updated, this, &AccountViewer::refresh);

    m_table=new PicsouTableWidget;
    ui->ops_layout->insertWidget(0, m_table);

    m_ops_stats=new OperationStatistics;
    m_ops_stats->append_field(m_rolling_expense_lab, tr("unknown"));
    ui->notes_layout->addWidget(m_ops_stats);

    /* payment methods */
    connect(ui->pm_add, &QPushButton::clicked, this, &AccountViewer::add_pm);
    connect(ui->action_add_pm, &QAction::triggered, this, &AccountViewer::add_pm);
    addAction(ui->action_add_pm);

    connect(ui->pm_edit, &QPushButton::clicked, this, &AccountViewer::edit_pm);
    connect(ui->action_edit_pm, &QAction::triggered, this, &AccountViewer::edit_pm);
    addAction(ui->action_edit_pm);

    connect(ui->pm_remove, &QPushButton::clicked, this, &AccountViewer::remove_pm);
    connect(ui->action_remove_pm, &QAction::triggered, this, &AccountViewer::remove_pm);
    addAction(ui->action_remove_pm);
    /* scheduled ops */
    connect(ui->sop_add, &QPushButton::clicked, this, &AccountViewer::add_sop);
    connect(ui->action_add_sop, &QAction::triggered, this, &AccountViewer::add_sop);
    addAction(ui->action_add_sop);

    connect(ui->sop_edit, &QPushButton::clicked, this, &AccountViewer::edit_sop);
    connect(ui->action_edit_sop, &QAction::triggered, this, &AccountViewer::edit_sop);
    addAction(ui->action_edit_sop);

    connect(ui->sop_remove, &QPushButton::clicked, this, &AccountViewer::remove_sop);
    connect(ui->action_remove_sop, &QAction::triggered, this, &AccountViewer::remove_sop);
    addAction(ui->action_remove_sop);
    /* ops */
    connect(ui->add_op, &QPushButton::clicked, this, &AccountViewer::add_op);
    connect(ui->action_add_op, &QAction::triggered, this, &AccountViewer::add_op);
    addAction(ui->action_add_op);

    connect(ui->edit_op, &QPushButton::clicked, this, &AccountViewer::edit_op);
    connect(ui->action_edit_op, &QAction::triggered, this, &AccountViewer::edit_op);
    connect(m_table, &PicsouTableWidget::cellDoubleClicked, this, &AccountViewer::table_edit_op);
    addAction(ui->action_edit_op);

    connect(ui->remove_op, &QPushButton::clicked, this, &AccountViewer::remove_op);
    connect(ui->action_remove_op, &QAction::triggered, this, &AccountViewer::remove_op);
    addAction(ui->action_remove_op);

    connect(ui->import_ops, &QPushButton::clicked, this, &AccountViewer::import_ops);
    connect(ui->action_import_ops, &QAction::triggered, this, &AccountViewer::import_ops);
    addAction(ui->action_import_ops);

    connect(ui->export_ops, &QPushButton::clicked, this, &AccountViewer::export_ops);
    connect(ui->action_export_ops, &QAction::triggered, this, &AccountViewer::export_ops);
    addAction(ui->action_export_ops);
}

void AccountViewer::refresh(const PicsouDBShPtr db)
{
    OperationCollection ops;
    AccountShPtr account=db->find_account(mod_obj_id());
    if(account.isNull()) {
        LOG_WARNING("failed to find account!");
        return;
    }
    /* payment methods */
    ui->payment_methods->clear();
    for(const auto &pm : account->payment_methods(true)) {
        new PicsouListItem(pm->name(), ui->payment_methods, pm->id());
    }
    bool has_pm=(ui->payment_methods->count());
    ui->pm_edit->setEnabled(has_pm);
    ui->pm_remove->setEnabled(has_pm);
    /* notes */
    ui->notes->setPlainText(account->notes());
    /* scheduled ops */
    ui->sops->clear();
    QString end;
    for(const auto &sop : account->scheduled_ops()) {
        end=(sop->schedule().endless()?tr("[endless]"):sop->schedule().until().toString(Qt::ISODate));
        new PicsouListItem(tr("[%0] %1 from %2 to %3 every %4 %5").arg(sop->name(),
                                                                       sop->amount().to_str(true),
                                                                       sop->schedule().from().toString(Qt::ISODate),
                                                                       end,
                                                                       QString::number(sop->schedule().freq_value()),
                                                                       Schedule::freq_unit2trstr(sop->schedule().freq_unit())),
                           ui->sops,
                           sop->id());
    }
    bool has_sops=(ui->sops->count());
    ui->sop_edit->setEnabled(has_sops);
    ui->sop_remove->setEnabled(has_sops);
    /* ops */
    ops=db->ops(mod_obj_id());
    QList<QStringList> budgets=ui_svc()->compute_budgets(ops, m_user_id);
    m_table->refresh(ops);
    m_ops_stats->refresh(ops.balance().to_str(true),
                         ops.total_debit().to_str(true),
                         ops.total_credit().to_str(true),
                         budgets);
    QDate today=QDate::currentDate();
    m_ops_stats->update_field(m_rolling_expense_lab,
                             ops.total_in_range(today.addDays(-30),
                                                today).to_str(true));
    bool has_ops=(ops.length()>0);
    ui->remove_op->setEnabled(has_ops);
    ui->edit_op->setEnabled(has_ops);
}

void AccountViewer::add_pm()
{
    ui_svc()->pm_add(mod_obj_id());
}

void AccountViewer::edit_pm()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->payment_methods->currentItem());
    if(item!=nullptr) {
        ui_svc()->pm_edit(mod_obj_id(), item->mod_obj_id());
    }
}

void AccountViewer::remove_pm()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->payment_methods->currentItem());
    if(item!=nullptr) {
        ui_svc()->pm_remove(mod_obj_id(), item->mod_obj_id());
    }
}

void AccountViewer::add_sop()
{
    ui_svc()->sop_add(m_user_id, mod_obj_id());
}

void AccountViewer::edit_sop()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->sops->currentItem());
    if(item!=nullptr) {
        ui_svc()->sop_edit(m_user_id, mod_obj_id(), item->mod_obj_id());
    }
}

void AccountViewer::remove_sop()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->sops->currentItem());
    if(item!=nullptr) {
        ui_svc()->sop_remove(mod_obj_id(), item->mod_obj_id());
    }
}

void AccountViewer::add_op()
{
    ui_svc()->op_add(m_user_id, mod_obj_id(), -1, -1);
}

void AccountViewer::edit_op()
{
    QUuid op_id;
    if(m_table->is_current_op_scheduled()) {
        ui_svc()->svc_op_failed(tr("Logical error: you can't edit a scheduled operation from this view."));
        return;
    }
    op_id=m_table->current_op();
    if(!op_id.isNull()) {
        ui_svc()->op_edit(m_user_id, mod_obj_id(), op_id, -1, -1);
    }
}

void AccountViewer::remove_op()
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

void AccountViewer::import_ops()
{
    ui_svc()->ops_import(mod_obj_id());
}

void AccountViewer::export_ops()
{
    ui_svc()->ops_export(mod_obj_id());
}

void AccountViewer::table_edit_op(int, int)
{
    edit_op();
}

