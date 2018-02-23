#include "picsoudbviewer.h"
#include "ui_picsoudbviewer.h"

#include "app/picsouuiservice.h"
#include "app/picsoumodelservice.h"

#include "ui/items/picsoulistitem.h"

PicsouDBViewer::~PicsouDBViewer()
{
    delete ui;
}

PicsouDBViewer::PicsouDBViewer(PicsouUIService *ui_svc,
                               QUuid uuid,
                               QWidget *parent) :
    PicsouUIViewer(ui_svc, uuid, parent),
    ui(new Ui::PicsouDBViewer)
{
    ui->setupUi(this);

    connect(ui_svc, &PicsouUIService::model_updated,
            this, &PicsouDBViewer::refresh);

    connect(ui->add_user, &QPushButton::clicked,
            this, &PicsouDBViewer::add_user);
    connect(ui->edit_user, &QPushButton::clicked,
            this, &PicsouDBViewer::edit_user);
    connect(ui->remove_user, &QPushButton::clicked,
            this, &PicsouDBViewer::remove_user);
}

void PicsouDBViewer::refresh(const PicsouDBPtr db)
{
    bool has_users;

    ui->name->setText(db->name());
    ui->version->setText(db->version());
    ui->description->setPlainText(db->description());

    ui->users_list->clear();
    foreach(UserPtr user, db->users(true)) {
        has_users=true;
        new PicsouListItem(user->name(), ui->users_list, user->id());
    }

    ui->add_user->setEnabled(true);
    ui->edit_user->setEnabled(has_users);
    ui->remove_user->setEnabled(has_users);
    if(has_users) {
        ui->users_list->setItemSelected(ui->users_list->item(0), true);
    }
}

void PicsouDBViewer::add_user()
{
    ui_svc()->user_add();
}

void PicsouDBViewer::edit_user()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->users_list->currentItem());
    ui_svc()->user_edit(item->mod_obj_id());
}

void PicsouDBViewer::remove_user()
{
    PicsouListItem *item;
    item=static_cast<PicsouListItem*>(ui->users_list->currentItem());
    ui_svc()->user_remove(item->mod_obj_id());
}

