#include "lockedobjectviewer.h"
#include "ui_lockedobjectviewer.h"

#include "app/picsouuiservice.h"

#include <QSvgWidget>

LockedObjectViewer::LockedObjectViewer(PicsouUIServicePtr ui_svc,
                                       QUuid uuid,
                                       QWidget *parent) :
    PicsouUIViewer(ui_svc, uuid, parent),
    ui(new Ui::LockedObjectViewer)
{
    ui->setupUi(this);

    QSvgWidget *svg=new QSvgWidget(":/resources/material-design/svg/folder-lock.svg", this);
    svg->setFixedSize(256, 256);

    ui->img_layout->insertWidget(ui->img_layout->indexOf(ui->img_rhs), svg);

    connect(ui_svc, &PicsouUIService::notify_model_updated, this, &LockedObjectViewer::refresh);
    connect(ui->unlock, &QPushButton::clicked, this, &LockedObjectViewer::unlock);
}

LockedObjectViewer::~LockedObjectViewer()
{
    delete ui;
}

void LockedObjectViewer::refresh(const PicsouDBPtr)
{

}

void LockedObjectViewer::unlock()
{
    ui_svc()->unlock(mod_obj_id());
}
