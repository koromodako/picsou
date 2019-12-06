#include "chartsform.h"
#include "ui_chartsform.h"

ChartsForm::ChartsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartsForm)
{
    ui->setupUi(this);
}

ChartsForm::~ChartsForm()
{
    delete ui;
}
