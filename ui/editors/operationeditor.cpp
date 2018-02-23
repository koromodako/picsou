#include "operationeditor.h"
#include "ui_operationeditor.h"

OperationEditor::~OperationEditor()
{
    delete ui;
}

OperationEditor::OperationEditor(double *amount,
                                 QDate *date,
                                 QString *payment_method,
                                 QString *budget,
                                 QString *recipient,
                                 QString *description,
                                 QWidget *parent) :
    QDialog(parent),
    _amount(amount),
    _date(date),
    _payment_method(payment_method),
    _budget(budget),
    _recipient(recipient),
    _description(description),
    ui(new Ui::OperationEditor)
{
    ui->setupUi(this);

    ui->budget->setEditable(false);

    connect(ui->buttons, &QDialogButtonBox::accepted,
            this, &OperationEditor::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected,
            this, &OperationEditor::reject);
}

void OperationEditor::set_budgets(const QStringList &budgets)
{
    ui->budget->clear();
    ui->budget->addItems(budgets);
}

void OperationEditor::set_payment_methods(const QStringList &payment_methods)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
}


void OperationEditor::accept()
{
    (*_amount)=ui->amount->value();
    (*_date)=ui->date->date();
    (*_payment_method)=ui->payment_method->currentText();
    (*_budget)=ui->budget->currentText();
    (*_recipient)=ui->recipient->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}
