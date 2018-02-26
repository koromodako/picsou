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
    bool has_accounts, has_budgets;
    UserPtr user=db->find_user(mod_obj_id());

    ui->accounts_list->clear();
    foreach (AccountPtr account, user->accounts(true)) {
        new PicsouListItem(account->name(), ui->accounts_list, account->id());
    }

    ui->budgets_list->clear();
    foreach (BudgetPtr budget, user->budgets(true)) {
        new PicsouListItem(budget->name(), ui->budgets_list, budget->id());
    }

    has_accounts=(ui->accounts_list->count()>0);
    has_budgets=(ui->budgets_list->count()>0);

    ui->edit_account->setEnabled(has_accounts);
    ui->remove_account->setEnabled(has_accounts);
    ui->edit_budget->setEnabled(has_budgets);
    ui->remove_budget->setEnabled(has_budgets);
}

void UserViewer::add_account()
{
    ui_svc()->account_add(mod_obj_id());
}

void UserViewer::edit_account()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->accounts_list->currentItem());
    if(item!=nullptr) {
        ui_svc()->account_edit(mod_obj_id(), item->mod_obj_id());
    }
}

void UserViewer::remove_account()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->accounts_list->currentItem());
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
    item=static_cast<PicsouListItem*>(ui->budgets_list->currentItem());
    if(item!=nullptr) {
        ui_svc()->budget_edit(mod_obj_id(), item->mod_obj_id());
    }
}

void UserViewer::remove_budget()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->budgets_list->currentItem());
    if(item!=nullptr) {
        ui_svc()->budget_remove(mod_obj_id(), item->mod_obj_id());
    }
}

