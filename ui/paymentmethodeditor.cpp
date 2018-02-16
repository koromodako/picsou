#include "paymentmethodeditor.h"
#include "ui_paymentmethodeditor.h"

PaymentMethodEditor::PaymentMethodEditor(PicsouUIService *ui_svc, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    ui(new Ui::PaymentMethodEditor)
{
    ui->setupUi(this);
}

PaymentMethodEditor::~PaymentMethodEditor()
{
    delete ui;
}
