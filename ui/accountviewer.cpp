#include "accountviewer.h"
#include "ui_accountviewer.h"

AccountViewer::~AccountViewer()
{
    delete ui;
}

AccountViewer::AccountViewer(PicsouUIService *ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::AccountViewer)
{
    ui->setupUi(this);
}

