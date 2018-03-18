#include "scheduledoperationeditor.h"
#include "ui_scheduledoperationeditor.h"

ScheduledOperationEditor::~ScheduledOperationEditor()
{
    delete ui;
}

ScheduledOperationEditor::ScheduledOperationEditor(double *amount,
                                                   QString *frequency,
                                                   QDate *date,
                                                   QString *payment_method,
                                                   QString *budget,
                                                   QString *recipient,
                                                   QString *description,
                                                   QWidget *parent) :
    QDialog(parent),
    _amount(amount),
    _frequency(frequency),
    _date(date),
    _payment_method(payment_method),
    _budget(budget),
    _recipient(recipient),
    _description(description),
    ui(new Ui::ScheduledOperationEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Scheduled Operation Editor"));

    ui->amount->setValue(*_amount);

    if(_date->isNull()) {
        ui->date->setDate(QDate::currentDate());
    } else {
        ui->date->setDate(*_date);
    }

    ui->budget->setEditable(false);
    ui->frequency->setEditable(false);
    ui->payment_method->setEditable(false);

    if(!_recipient->isNull()) {
        ui->recipient->setText(*_recipient);
    }

    if(!_description->isNull()) {
        ui->description->setPlainText(*_description);
    }

    connect(ui->save, &QPushButton::clicked,
            this, &ScheduledOperationEditor::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &ScheduledOperationEditor::reject);
}

void ScheduledOperationEditor::set_budgets(const QStringList &budgets)
{
    ui->budget->clear();
    ui->budget->addItems(budgets);
    if(!_budget->isNull()) {
        ui->budget->setCurrentText(*_budget);
    }
}

void ScheduledOperationEditor::set_frequencies(const QStringList &frequencies)
{
    ui->budget->clear();
    ui->budget->addItems(frequencies);
    if(!_frequency->isNull()) {
        ui->frequency->setCurrentText(*_frequency);
    }
}

void ScheduledOperationEditor::set_payment_methods(const QStringList &payment_methods)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
    if(!_payment_method->isNull()) {
        ui->payment_method->setCurrentText(*_payment_method);
    }
}

void ScheduledOperationEditor::accept()
{
    (*_amount)=ui->amount->value();
    (*_frequency)=ui->frequency->currentText();
    (*_date)=ui->date->date();
    (*_payment_method)=ui->payment_method->currentText();
    (*_budget)=ui->budget->currentText();
    (*_recipient)=ui->recipient->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}
