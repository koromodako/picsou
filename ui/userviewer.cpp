#include "userviewer.h"
#include "ui_userviewer.h"

UserViewer::UserViewer(PicsouUIService *ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::UserViewer)
{
    ui->setupUi(this);
}

UserViewer::~UserViewer()
{
    delete ui;
}
