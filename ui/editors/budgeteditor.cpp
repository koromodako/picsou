#include "budgeteditor.h"
#include "ui_budgeteditor.h"

BudgetEditor::~BudgetEditor()
{
    delete ui;
}


BudgetEditor::BudgetEditor(double *amount,
                           QString *name,
                           QString *description,
                           QWidget *parent) :
    QDialog(parent),
    _amount(amount),
    _name(name),
    _description(description),
    ui(new Ui::BudgetEditor)
{
    ui->setupUi(this);

    connect(ui->buttons, &QDialogButtonBox::accepted,
            this, &BudgetEditor::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected,
            this, &BudgetEditor::reject);
}

void BudgetEditor::accept()
{
    (*_amount)=ui->amount->value();
    (*_name)=ui->name->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}
