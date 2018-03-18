#include "paymentmethodeditor.h"
#include "ui_paymentmethodeditor.h"

PaymentMethodEditor::PaymentMethodEditor(QString *name, QWidget *parent) :
    QDialog(parent),
    _name(name),
    ui(new Ui::PaymentMethodEditor)
{
    ui->setupUi(this);

    setWindowTitle(tr("Payment Method Editor"));

    if(!_name->isNull()) {
        ui->name->setText(*_name);
    }

    connect(ui->save, &QPushButton::clicked,
            this, &PaymentMethodEditor::accept);
    connect(ui->cancel, &QPushButton::clicked,
            this, &PaymentMethodEditor::reject);
}

void PaymentMethodEditor::accept()
{
    (*_name)=ui->name->text();
    QDialog::accept();
}

PaymentMethodEditor::~PaymentMethodEditor()
{
    delete ui;
}
