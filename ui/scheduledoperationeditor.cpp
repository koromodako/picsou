#include "scheduledoperationeditor.h"
#include "ui_scheduledoperationeditor.h"

ScheduledOperationEditor::ScheduledOperationEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScheduledOperationEditor)
{
    ui->setupUi(this);
}

ScheduledOperationEditor::~ScheduledOperationEditor()
{
    delete ui;
}
