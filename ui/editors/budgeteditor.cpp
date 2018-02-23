#include "budgeteditor.h"
#include "ui_budgeteditor.h"

BudgetEditor::~BudgetEditor()
{
    delete ui;
}

BudgetEditor::BudgetEditor(PicsouUIService *ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::BudgetEditor)
{
    ui->setupUi(this);
}

