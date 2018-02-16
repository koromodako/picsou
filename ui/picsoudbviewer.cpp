#include "picsoudbviewer.h"
#include "ui_picsoudbviewer.h"

PicsouDBViewer::PicsouDBViewer(PicsouUIService *ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::PicsouDBViewer)
{
    ui->setupUi(this);
}

PicsouDBViewer::~PicsouDBViewer()
{
    delete ui;
}
