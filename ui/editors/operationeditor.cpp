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

    setWindowTitle(tr("Operation Editor"));

    ui->budget->setEditable(false);

    ui->amount->setValue(*_amount);
    if(!_date->isNull()){
        ui->date->setDate(*_date);
    }
    if(!_recipient->isNull()){
        ui->recipient->setText(*_recipient);
    }
    if(!_description->isNull()){
        ui->description->setPlainText(*_description);
    }

    connect(ui->save, &QPushButton::clicked,
            this, &OperationEditor::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &OperationEditor::reject);
}

void OperationEditor::set_budgets(const QStringList &budgets,
                                  const QString &current)
{
    ui->budget->clear();
    ui->budget->addItems(budgets);
    if(!current.isNull()){
        ui->budget->setCurrentText(current);
    }
}

void OperationEditor::set_payment_methods(const QStringList &payment_methods,
                                          const QString &current)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
    if(!current.isNull()){
        ui->payment_method->setCurrentText(current);
    }
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
