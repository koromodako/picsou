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
#include "picsoudbviewer.h"
#include "ui_picsoudbviewer.h"

#include "app/picsouuiservice.h"
#include "app/picsoumodelservice.h"

#include "ui/items/picsoulistitem.h"

PicsouDBViewer::~PicsouDBViewer()
{
    delete ui;
}

PicsouDBViewer::PicsouDBViewer(PicsouUIServicePtr ui_svc,
                               QUuid uuid,
                               QWidget *parent) :
    PicsouUIViewer(ui_svc, uuid, parent),
    ui(new Ui::PicsouDBViewer)
{
    ui->setupUi(this);
    connect(ui_svc, &PicsouUIService::notify_model_updated, this, &PicsouDBViewer::refresh);
    /* user editor */
    connect(ui->add_user, &QPushButton::clicked, this, &PicsouDBViewer::add_user);
    connect(ui->action_add_user, &QAction::triggered, this, &PicsouDBViewer::add_user);
    addAction(ui->action_add_user);
    connect(ui->edit_user, &QPushButton::clicked, this, &PicsouDBViewer::edit_user);
    connect(ui->action_edit_user, &QAction::triggered, this, &PicsouDBViewer::edit_user);
    addAction(ui->action_edit_user);
    connect(ui->remove_user, &QPushButton::clicked, this, &PicsouDBViewer::remove_user);
    connect(ui->action_remove_user, &QAction::triggered, this, &PicsouDBViewer::remove_user);
    addAction(ui->action_remove_user);
}

void PicsouDBViewer::refresh(const PicsouDBShPtr db)
{
    bool has_users=false;

    ui->name->setText(db->name());
    ui->version->setText(db->version().to_str());
    ui->timestamp->setText(db->timestamp().toString(Qt::ISODate));
    ui->description->setPlainText(db->description());

    ui->users_list->clear();
    for(const auto &user : db->users(true)) {
        has_users=true;
        new PicsouListItem(user->name(), ui->users_list, user->id());
    }

    ui->add_user->setEnabled(true);
    ui->edit_user->setEnabled(has_users);
    ui->remove_user->setEnabled(has_users);
}

void PicsouDBViewer::add_user()
{
    ui_svc()->user_add();
}

void PicsouDBViewer::edit_user()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->users_list->currentItem());
    if(item!=nullptr) {
        ui_svc()->user_edit(item->mod_obj_id());
    }
}

void PicsouDBViewer::remove_user()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->users_list->currentItem());
    if(item!=nullptr) {
        ui_svc()->user_remove(item->mod_obj_id());
    }
}

