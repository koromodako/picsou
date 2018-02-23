#include "scheduledoperationeditor.h"
#include "ui_scheduledoperationeditor.h"

ScheduledOperationEditor::~ScheduledOperationEditor()
{
    delete ui;
}

ScheduledOperationEditor::ScheduledOperationEditor(double *amount,
                                                   QString *freq,
                                                   QDate *date,
                                                   QString *payment_method,
                                                   QString *budget,
                                                   QString *recipient,
                                                   QString *description,
                                                   QWidget *parent) :
    QDialog(parent),
    _amount(amount),
    _freq(freq),
    _date(date),
    _payment_method(payment_method),
    _budget(budget),
    _recipient(recipient),
    _description(description),
    ui(new Ui::ScheduledOperationEditor)
{
    ui->setupUi(this);

    connect(ui->buttons, &QDialogButtonBox::accepted,
            this, &ScheduledOperationEditor::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected,
            this, &ScheduledOperationEditor::reject);
}

void ScheduledOperationEditor::set_budgets(const QStringList &budgets)
{
    ui->budget->clear();
    ui->budget->addItems(budgets);
}

void ScheduledOperationEditor::set_frequencies(const QStringList &frequencies)
{
    ui->budget->clear();
    ui->budget->addItems(frequencies);
}

void ScheduledOperationEditor::set_payment_methods(const QStringList &payment_methods)
{
    ui->payment_method->clear();
    ui->payment_method->addItems(payment_methods);
}

void ScheduledOperationEditor::accept()
{
    (*_amount)=ui->amount->value();
    (*_freq)=ui->frequency->currentText();
    (*_date)=ui->date->date();
    (*_payment_method)=ui->payment_method->currentText();
    (*_budget)=ui->budget->currentText();
    (*_recipient)=ui->recipient->text();
    (*_description)=ui->description->toPlainText();
    QDialog::accept();
}
