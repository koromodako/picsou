#ifndef PAYMENTMETHODEDITOR_H
#define PAYMENTMETHODEDITOR_H

#include <QWidget>

namespace Ui {
class PaymentMethodEditor;
}

class PaymentMethodEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentMethodEditor(QWidget *parent = 0);
    ~PaymentMethodEditor();

private:
    Ui::PaymentMethodEditor *ui;
};

#endif // PAYMENTMETHODEDITOR_H
