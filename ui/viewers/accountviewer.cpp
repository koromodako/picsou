#include "accountviewer.h"
#include "ui_accountviewer.h"

#include "app/picsouuiservice.h"
#include "ui/items/picsoulistitem.h"

AccountViewer::~AccountViewer()
{
    delete ui;
}

AccountViewer::AccountViewer(PicsouUIService *ui_svc,
                             QUuid account_uuid,
                             QWidget *parent) :
    PicsouUIViewer(ui_svc, account_uuid, parent),
    ui(new Ui::AccountViewer)
{
    ui->setupUi(this);

    connect(ui_svc, &PicsouUIService::model_updated,
            this, &AccountViewer::refresh);
    /* payment methods */
    connect(ui->pm_add, &QPushButton::clicked,
            this, &AccountViewer::add_pm);
    connect(ui->pm_edit, &QPushButton::clicked,
            this, &AccountViewer::edit_pm);
    connect(ui->pm_remove, &QPushButton::clicked,
            this, &AccountViewer::remove_pm);
    /* scheduled ops */
    ui->sops_table->setEnabled(false);
    ui->sops_add->setEnabled(false);
    ui->sops_edit->setEnabled(false);
    ui->sops_remove->setEnabled(false);
    ui->scheduled_ops_group->setVisible(false);
}

void AccountViewer::refresh(const PicsouDBPtr db)
{
    bool has_pm;
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
