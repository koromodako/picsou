#include "operationeditor.h"
#include "ui_operationeditor.h"

OperationEditor::~OperationEditor()
{
    delete ui;
}

OperationEditor::OperationEditor(QDate *date,
                                 Amount *amount,
                                 QString *payment_method,
                                 QString *budget,
                                 QString *recipient,
                                 QString *description,
                                 int year,
                                 int month,
                                 QWidget *parent) :
    QDialog(parent),
    _date(date),
    _amount(amount),
    _payment_method(payment_method),
    _budget(budget),
    _recipient(recipient),
    _description(description),
    _year(year),
    _month(month),
    ui(new Ui::OperationEditor)
{
    QDate min, max;

    ui->setupUi(this);

    setWindowTitle(tr("Operation Editor"));

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));
    ui->amount->setValue(_amount->value());

    if(!_date->isNull()){
        ui->date->setDate(*_date);
    } else {
        ui->date->setDate(QDate::currentDate());
    }

    if(_year>0) {
        min.setDate(_year, 1, 1);
        max.setDate(_year, 12, 31);

        if(_month>0) {
            min.setDate(_year, _month, 1);
            max.setDate(_year, _month, min.daysInMonth());
        }

        ui->date->setDateRange(min, max);
    }

    ui->budget->setEditable(false);
    ui->payment_method->setEditable(false);

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

void OperationEditor::set_budgets(const QStringList &budgets)
{
    ui->budget->clear();
    ui->budget->addItems(budgets);
    if(!_budget->isNull()){
        ui->budget->setCurrentText(*_budget);
    }
}

void OperationEditor::set_payment_methods(const QStringList &payment_methods)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
    if(!_payment_method->isNull()){
        ui->payment_method->setCurrentText(*_payment_method);
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
