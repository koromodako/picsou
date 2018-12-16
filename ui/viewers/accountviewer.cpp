#include "accountviewer.h"
#include "ui_accountviewer.h"

#include "utils/macro.h"

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
    connect(ui->sop_add, &QPushButton::clicked,
            this, &AccountViewer::add_sop);
    connect(ui->action_add_sop, &QAction::triggered,
            this, &AccountViewer::add_sop);
    addAction(ui->action_add_sop);

    connect(ui->sop_edit, &QPushButton::clicked,
            this, &AccountViewer::edit_sop);
    connect(ui->action_edit_sop, &QAction::triggered,
            this, &AccountViewer::edit_sop);
    addAction(ui->action_edit_sop);

    connect(ui->sop_remove, &QPushButton::clicked,
            this, &AccountViewer::remove_sop);
    connect(ui->action_remove_sop, &QAction::triggered,
            this, &AccountViewer::remove_sop);
    addAction(ui->action_remove_sop);
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
    OperationCollection ops;
    AccountPtr account=db->find_account(mod_obj_id());
    /* payment methods */
    ui->payment_methods->clear();
    foreach(PaymentMethodPtr pm, account->payment_methods(true)) {
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
    foreach(ScheduledOperationPtr sop, account->scheduled_ops()) {
        end=(sop->schedule().endless()?tr("[endless]"):sop->schedule().until().toString(Qt::ISODate));
        new PicsouListItem(tr("[%0] %1 from %2 to %3 each %4 %5").arg(sop->name(),
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
    _table->refresh(ops);
    _ops_stats->refresh(ops);
    QDate today=QDate::currentDate();
    _ops_stats->update_field(_rolling_expense_lab,
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
    ui_svc()->sop_add(_user_id, mod_obj_id());
}

void AccountViewer::edit_sop()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->sops->currentItem());
    if(item!=nullptr) {
        ui_svc()->sop_edit(_user_id, mod_obj_id(), item->mod_obj_id());
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

