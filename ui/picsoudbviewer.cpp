#include "picsoudbviewer.h"
#include "ui_picsoudbviewer.h"

PicsouDBViewer::PicsouDBViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicsouDBViewer)
{
    ui->setupUi(this);
}

PicsouDBViewer::~PicsouDBViewer()
{
    delete ui;
}
