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

    /* user editor */
    connect(ui->add_user, &QPushButton::clicked,
            this, &PicsouDBViewer::add_user);
    connect(ui->action_add_user, &QAction::triggered,
            this, &PicsouDBViewer::add_user);
    addAction(ui->action_add_user);

    connect(ui->edit_user, &QPushButton::clicked,
            this, &PicsouDBViewer::edit_user);
    connect(ui->action_edit_user, &QAction::triggered,
            this, &PicsouDBViewer::edit_user);
    addAction(ui->action_edit_user);

    connect(ui->remove_user, &QPushButton::clicked,
            this, &PicsouDBViewer::remove_user);
    connect(ui->action_remove_user, &QAction::triggered,
            this, &PicsouDBViewer::remove_user);
    addAction(ui->action_remove_user);
}

void PicsouDBViewer::refresh(const PicsouDBPtr db)
{
    bool has_users=false;

    ui->name->setText(db->name());
    ui->version->setText(db->version().to_str());
    ui->timestamp->setText(db->timestamp().toString(Qt::ISODate));
    ui->description->setPlainText(db->description());

    ui->users_list->clear();
    foreach(UserPtr user, db->users(true)) {
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

