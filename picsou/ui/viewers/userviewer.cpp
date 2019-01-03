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
#include "userviewer.h"
#include "ui_userviewer.h"
#include "utils/macro.h"
#include "app/picsouuiservice.h"
#include "ui/items/picsoulistitem.h"

UserViewer::~UserViewer()
{
    delete ui;
}

UserViewer::UserViewer(PicsouUIServicePtr ui_svc,
                       QUuid uuid,
                       QWidget *parent) :
    PicsouUIViewer(ui_svc, uuid, parent),
    ui(new Ui::UserViewer)
{
    ui->setupUi(this);

    connect(ui_svc, &PicsouUIService::notify_model_updated, this, &UserViewer::refresh);
    /* budget editor */
    connect(ui->add_budget, &QPushButton::clicked, this, &UserViewer::add_budget);
    connect(ui->action_add_budget, &QAction::triggered, this, &UserViewer::add_budget);
    addAction(ui->action_add_budget);
    connect(ui->edit_budget, &QPushButton::clicked, this, &UserViewer::edit_budget);
    connect(ui->action_edit_budget, &QAction::triggered, this, &UserViewer::edit_budget);
    addAction(ui->action_edit_budget);
    connect(ui->remove_budget, &QPushButton::clicked, this, &UserViewer::remove_budget);
    connect(ui->action_remove_budget, &QAction::triggered, this, &UserViewer::remove_budget);
    addAction(ui->action_remove_budget);
    /* account editor */
    connect(ui->add_account, &QPushButton::clicked, this, &UserViewer::add_account);
    connect(ui->action_add_account, &QAction::triggered, this, &UserViewer::add_account);
    addAction(ui->action_add_account);
    connect(ui->edit_account, &QPushButton::clicked, this, &UserViewer::edit_account);
    connect(ui->action_edit_account, &QAction::triggered, this, &UserViewer::edit_account);
    addAction(ui->action_edit_account);
    connect(ui->remove_account, &QPushButton::clicked, this, &UserViewer::remove_account);
    connect(ui->action_remove_account, &QAction::triggered, this, &UserViewer::remove_account);
    addAction(ui->action_remove_account);
    /* transfer */
    connect(ui->transfer, &QPushButton::clicked, this, &UserViewer::transfer);
    connect(ui->action_transfer, &QAction::triggered, this, &UserViewer::transfer);
    addAction(ui->action_transfer);
}

void UserViewer::refresh(const PicsouDBShPtr db)
{
    bool has_accounts, has_budgets;
    UserShPtr user=db->find_user(mod_obj_id());
    if(user.isNull()) {
        LOG_WARNING("failed to find user!");
        return;
    }
    ui->accounts_list->clear();
    for(const auto &account : user->accounts(true)) {
        new PicsouListItem(account->name(), ui->accounts_list, account->id());
    }

    ui->budgets_list->clear();
    for(const auto &budget : user->budgets(true)) {
        new PicsouListItem(tr("%0 (%1)").arg(budget->name(), budget->amount().to_str(true)),
                           ui->budgets_list, budget->id());
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

void UserViewer::transfer()
{
    ui_svc()->transfer_add(mod_obj_id());
}

