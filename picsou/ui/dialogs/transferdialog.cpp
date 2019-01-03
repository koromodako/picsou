#include "transferdialog.h"
#include "ui_transferdialog.h"

#include <QMessageBox>

TransferDialog::~TransferDialog()
{
    delete ui;
}

TransferDialog::TransferDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransferDialog)
{
    ui->setupUi(this);

    m_schedule_form=new ScheduleForm;
    ui->schedule_box->layout()->addWidget(m_schedule_form);

    ui->sender->setEditable(false);
    ui->recipient->setEditable(false);

    ui->date->setDate(QDate::currentDate());

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));

    connect(ui->schedule_box, &QGroupBox::toggled, this, &TransferDialog::update_scheduled);
}

void TransferDialog::set_accounts(const QStringList &accounts)
{
    ui->sender->clear();
    ui->sender->addItems(accounts);
    ui->recipient->clear();
    ui->recipient->addItems(accounts);
}

bool TransferDialog::scheduled() const
{
    return ui->schedule_box->isChecked();
}

void TransferDialog::accept()
{
    m_sender=ui->sender->currentText();
    m_recipient=ui->recipient->currentText();
    if(m_sender==m_recipient) {
        QMessageBox::critical(this, tr("Logical error"), tr("You cannot transfer using the same account."));
        return;
    }
    m_date=ui->date->date();
    m_amount=ui->amount->value();
    m_description=ui->description->toPlainText();
    m_schedule=m_schedule_form->schedule();
    QDialog::accept();
}

void TransferDialog::update_scheduled(bool scheduled)
{
    ui->date->setEnabled(!scheduled);
}
