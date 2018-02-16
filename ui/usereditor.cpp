#include "usereditor.h"
#include "ui_usereditor.h"

UserEditor::UserEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserEditor)
{
    ui->setupUi(this);
}

UserEditor::~UserEditor()
{
    delete ui;
}
