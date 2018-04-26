#include "accountviewer.h"
#include "ui_accountviewer.h"

#include "app/picsouuiservice.h"
#include "ui/items/picsoulistitem.h"
#include "ui/viewers/operationviewer.h"

AccountViewer::~AccountViewer()
{
    delete _ops_stats;
    delete _table;
    delete ui;
}

AccountViewer::AccountViewer(PicsouUIService *ui_svc,
                             QUuid user_uuid,
                             QUuid account_uuid,
                             QWidget *parent) :
    PicsouUIViewer(ui_svc, account_uuid, parent),
   _user_id(user_uuid),
   _rolling_expense_lab(tr("Rolling expense (30 days):")),
    ui(new Ui::AccountViewer)
{
    ui->setupUi(this);
    connect(ui_svc, &PicsouUIService::model_updated,
            this, &AccountViewer::refresh);

    _table = new PicsouTableWidget;
    ui->ops_layout->insertWidget(0, _table);

    _ops_stats = new OperationStatistics;
    _ops_stats->append_field(_rolling_expense_lab, tr("unknown"));
    ui->hlayout->addWidget(_ops_stats);

    /* payment methods */
    connect(ui->pm_add, &QPushButton::clicked,
            this, &AccountViewer::add_pm);
    connect(ui->action_add_pm, &QAction::triggered,
            this, &AccountViewer::add_pm);
    addAction(ui->action_add_pm);

    connect(ui->pm_edit, &QPushButton::clicked,
            this, &AccountViewer::edit_pm);
    connect(ui->action_edit_pm, &QAction::triggered,
            this, &AccountViewer::edit_pm);
    addAction(ui->action_edit_pm);

    connect(ui->pm_remove, &QPushButton::clicked,
            this, &AccountViewer::remove_pm);
    connect(ui->action_remove_pm, &QAction::triggered,
            this, &AccountViewer::remove_pm);
    addAction(ui->action_remove_pm);
    /* scheduled ops */
    ui->sops_table->setEnabled(false);
    ui->sops_add->setEnabled(false);
    ui->sops_edit->setEnabled(false);
    ui->sops_remove->setEnabled(false);
    ui->scheduled_ops_group->setVisible(false);
    /* ops */
    connect(ui->add_op, &QPushButton::clicked,
            this, &AccountViewer::add_op);
    connect(ui->action_add_op, &QAction::triggered,
            this, &AccountViewer::add_op);
    addAction(ui->action_add_op);

    connect(ui->edit_op, &QPushButton::clicked,
            this, &AccountViewer::edit_op);
    connect(ui->action_edit_op, &QAction::triggered,
            this, &AccountViewer::edit_op);
    connect(_table, &PicsouTableWidget::cellDoubleClicked,
            this, &AccountViewer::table_edit_op);
    addAction(ui->action_edit_op);

    connect(ui->remove_op, &QPushButton::clicked,
            this, &AccountViewer::remove_op);
    connect(ui->action_remove_op, &QAction::triggered,
            this, &AccountViewer::remove_op);
    addAction(ui->action_remove_op);

    connect(ui->import_ops, &QPushButton::clicked,
            this, &AccountViewer::import_ops);
    connect(ui->action_import_ops, &QAction::triggered,
            this, &AccountViewer::import_ops);
    addAction(ui->action_import_ops);

    connect(ui->export_ops, &QPushButton::clicked,
            this, &AccountViewer::export_ops);
    connect(ui->action_export_ops, &QAction::triggered,
            this, &AccountViewer::export_ops);
    addAction(ui->action_export_ops);
}

void AccountViewer::refresh(const PicsouDBPtr db)
{
    bool has_pm, has_ops;
    QDate today;
    OperationCollection ops;
    AccountPtr account=db->find_account(mod_obj_id());
    /* payment methods */
    ui->payment_methods->clear();
    foreach (PaymentMethodPtr pm, account->payment_methods(true)) {
        new PicsouListItem(pm->name(), ui->payment_methods, pm->id());
    }
    has_pm=(ui->payment_methods->count());
    ui->pm_edit->setEnabled(has_pm);
    ui->pm_remove->setEnabled(has_pm);
    /* scheduled ops */
    /*
    foreach (ScheduledOperationPtr sop, account->scheduled_ops()) {
        TODO
    }
    */
    /* ops */
    ops=db->ops(mod_obj_id());
    _table->refresh(ops);
    _ops_stats->refresh(ops);
    today=QDate::currentDate();
    _ops_stats->update_field(_rolling_expense_lab,
                             ops.total_in_range(today.addDays(-30),
                                                today).to_str(true));

    has_ops=(ops.length()>0);
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

void AccountViewer::add_op()
{
    ui_svc()->op_add(_user_id, mod_obj_id(), -1, -1);
}

void AccountViewer::edit_op()
{
    QUuid op_id;
    op_id=_table->current_op();
    if(!op_id.isNull()) {
        ui_svc()->op_edit(_user_id, mod_obj_id(), op_id, -1, -1);
    }
}

void AccountViewer::remove_op()
{
    QUuid op_id;
    op_id=_table->current_op();
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

