#include "accountviewer.h"
#include "ui_accountviewer.h"

#include "app/picsouuiservice.h"
#include "ui/items/picsoulistitem.h"

AccountViewer::~AccountViewer()
{
    delete ui;
}

AccountViewer::AccountViewer(PicsouUIService *ui_svc,
                             QUuid uuid,
                             QWidget *parent) :
    PicsouUIViewer(ui_svc, uuid, parent),
    ui(new Ui::AccountViewer)
{
    ui->setupUi(this);

    connect(ui_svc, &PicsouUIService::model_updated,
            this, &AccountViewer::refresh);


    /* payment methods */
    connect(ui->payment_methods_add, &QPushButton::clicked,
            this, &AccountViewer::add_pm);
    connect(ui->payment_methods_edit, &QPushButton::clicked,
            this, &AccountViewer::edit_pm);
    connect(ui->payment_methods_remove, &QPushButton::clicked,
            this, &AccountViewer::remove_pm);
    /* scheduled ops */
    ui->scheduled_ops_table->setEnabled(false);
    ui->scheduled_ops_add->setEnabled(false);
    ui->scheduled_ops_edit->setEnabled(false);
    ui->scheduled_ops_remove->setEnabled(false);
    ui->scheduled_ops_group->setVisible(false);
}

void AccountViewer::refresh(const PicsouDBPtr db)
{
    AccountPtr account=db->find_account(mod_obj_id());
    /* payment methods */
    ui->payment_methods_list->clear();
    foreach (PaymentMethodPtr pm, account->payment_methods(true)) {
        new PicsouListItem(pm->name(), ui->payment_methods_list, pm->id());
    }
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
    item=static_cast<PicsouListItem*>(ui->payment_methods_list->currentItem());
    ui_svc()->pm_edit(mod_obj_id(), item->mod_obj_id());
}

void AccountViewer::remove_pm()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->payment_methods_list->currentItem());
    ui_svc()->pm_remove(mod_obj_id(), item->mod_obj_id());
}
