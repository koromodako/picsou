#include "transferdialog.h"
#include "ui_transferdialog.h"

TransferDialog::~TransferDialog()
{
    delete ui;
}

TransferDialog::TransferDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransferDialog)
{
    ui->setupUi(this);

    ui->sender->setEditable(false);
    ui->recipient->setEditable(false);

    ui->amount->setPrefix(tr("$"));
    ui->amount->setSuffix(tr(" "));

    ui->freq_value->setMinimum(1);

    ui->freq_unit->addItems(Schedule::TR_FREQUENCY_UNITS);
    ui->freq_unit->setEditable(false);
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
    return ui->schedule->isChecked();
}

void TransferDialog::accept()
{
    m_sender=ui->sender->currentText();
    m_recipient=ui->recipient->currentText();
    m_amount=ui->amount->value();
    m_description=ui->description->toPlainText();
    m_schedule.update(ui->from->date(),
                      ui->until->date(),
                      ui->endless,
                      ui->freq_value->value(),
                      ui->freq_unit->currentText());
}

