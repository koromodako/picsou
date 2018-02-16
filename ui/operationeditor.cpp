#include "operationeditor.h"
#include "ui_operationeditor.h"

OperationEditor::OperationEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperationEditor)
{
    ui->setupUi(this);
}

OperationEditor::~OperationEditor()
{
    delete ui;
}
