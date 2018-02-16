#include "accounteditor.h"
#include "ui_accounteditor.h"

AccountEditor::AccountEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountEditor)
{
    ui->setupUi(this);
}

AccountEditor::~AccountEditor()
{
    delete ui;
}
