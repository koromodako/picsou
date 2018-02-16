#include "accountviewer.h"
#include "ui_accountviewer.h"

AccountViewer::AccountViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountViewer)
{
    ui->setupUi(this);
}

AccountViewer::~AccountViewer()
{
    delete ui;
}
