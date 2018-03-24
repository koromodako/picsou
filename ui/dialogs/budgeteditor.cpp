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

    setWindowTitle(tr("Budget Editor"));

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));
    ui->amount->setValue(*_amount);

    if(!_name->isNull()) {
        ui->name->setText(*_name);
    }
    if(!_description->isNull()) {
        ui->description->setPlainText(*_description);
    }

    connect(ui->save, &QPushButton::clicked,
            this, &BudgetEditor::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &BudgetEditor::reject);
}

void BudgetEditor::accept()
{
    (*_amount)=ui->amount->value();
    (*_name)=ui->name->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}
