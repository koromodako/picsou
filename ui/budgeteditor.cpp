#include "budgeteditor.h"
#include "ui_budgeteditor.h"

BudgetEditor::BudgetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BudgetEditor)
{
    ui->setupUi(this);
}

BudgetEditor::~BudgetEditor()
{
    delete ui;
}
