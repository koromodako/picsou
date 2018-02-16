#include "scheduledoperationeditor.h"
#include "ui_scheduledoperationeditor.h"

ScheduledOperationEditor::~ScheduledOperationEditor()
{
    delete ui;
}

ScheduledOperationEditor::ScheduledOperationEditor(PicsouUIService *ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::ScheduledOperationEditor)
{
    ui->setupUi(this);
}

