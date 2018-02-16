#include "paymentmethodeditor.h"
#include "ui_paymentmethodeditor.h"

PaymentMethodEditor::PaymentMethodEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentMethodEditor)
{
    ui->setupUi(this);
}

PaymentMethodEditor::~PaymentMethodEditor()
{
    delete ui;
}
