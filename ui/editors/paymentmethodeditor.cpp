#include "paymentmethodeditor.h"
#include "ui_paymentmethodeditor.h"

PaymentMethodEditor::PaymentMethodEditor(QString *name, QWidget *parent) :
    QDialog(parent),
    _name(name),
    ui(new Ui::PaymentMethodEditor)
{
    ui->setupUi(this);

    connect(ui->buttons, &QDialogButtonBox::accepted,
            this, &PaymentMethodEditor::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected,
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
