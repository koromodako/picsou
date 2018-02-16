#ifndef PAYMENTMETHODEDITOR_H
#define PAYMENTMETHODEDITOR_H

#include <QWidget>

#include "picsouui.h"

namespace Ui {
class PaymentMethodEditor;
}

class PaymentMethodEditor : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~PaymentMethodEditor();
    explicit PaymentMethodEditor(PicsouUIService *ui_svc, QWidget *parent = 0);

private:
    Ui::PaymentMethodEditor *ui;
};

#endif // PAYMENTMETHODEDITOR_H
