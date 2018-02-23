#include "operationeditor.h"
#include "ui_operationeditor.h"

OperationEditor::OperationEditor(PicsouUIService *ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::OperationEditor)
{
    ui->setupUi(this);
}

OperationEditor::~OperationEditor()
{
    delete ui;
}
