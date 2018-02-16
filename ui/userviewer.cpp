#include "userviewer.h"
#include "ui_userviewer.h"

UserViewer::UserViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserViewer)
{
    ui->setupUi(this);
}

UserViewer::~UserViewer()
{
    delete ui;
}
