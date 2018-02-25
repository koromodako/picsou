#include "userviewer.h"
#include "ui_userviewer.h"

#include "app/picsouuiservice.h"
#include "ui/items/picsoulistitem.h"

UserViewer::~UserViewer()
{
    delete ui;
}

UserViewer::UserViewer(PicsouUIService *ui_svc,
                       QUuid uuid,
                       QWidget *parent) :
    PicsouUIViewer(ui_svc, uuid, parent),
    ui(new Ui::UserViewer)
{
    ui->setupUi(this);

    connect(ui_svc, &PicsouUIService::model_updated,
            this, &UserViewer::refresh);

    connect(ui->add_account, &QPushButton::clicked,
            this, &UserViewer::add_account);
    connect(ui->edit_account, &QPushButton::clicked,
            this, &UserViewer::edit_account);
    connect(ui->remove_account, &QPushButton::clicked,
            this, &UserViewer::remove_account);

    connect(ui->add_budget, &QPushButton::clicked,
            this, &UserViewer::add_budget);
    connect(ui->edit_budget, &QPushButton::clicked,
            this, &UserViewer::edit_budget);
    connect(ui->remove_budget, &QPushButton::clicked,
            this, &UserViewer::remove_budget);
}

void UserViewer::refresh(const PicsouDBPtr db)
{
    UserPtr user=db->find_user(mod_obj_id());

    ui->list_accounts->clear();
    foreach (AccountPtr account, user->accounts(true)) {
        new PicsouListItem(account->name(), ui->list_accounts, account->id());
    }

    ui->list_budgets->clear();
    foreach (BudgetPtr budget, user->budgets(true)) {
        new PicsouListItem(budget->name(), ui->list_budgets, budget->id());
    }
}

void UserViewer::add_account()
{
    ui_svc()->account_add(mod_obj_id());
}

void UserViewer::edit_account()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->list_accounts->currentItem());
    if(item!=nullptr) {
        ui_svc()->account_edit(mod_obj_id(), item->mod_obj_id());
    }
}

void UserViewer::remove_account()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->list_accounts->currentItem());
    if(item!=nullptr) {
        ui_svc()->account_remove(mod_obj_id(), item->mod_obj_id());
    }
}

void UserViewer::add_budget()
{
    ui_svc()->budget_add(mod_obj_id());
}

void UserViewer::edit_budget()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->list_budgets->currentItem());
    if(item!=nullptr) {
        ui_svc()->budget_edit(mod_obj_id(), item->mod_obj_id());
    }
}

void UserViewer::remove_budget()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->list_budgets->currentItem());
    if(item!=nullptr) {
        ui_svc()->budget_remove(mod_obj_id(), item->mod_obj_id());
    }
}

