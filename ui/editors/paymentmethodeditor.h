#ifndef PAYMENTMETHODEDITOR_H
#define PAYMENTMETHODEDITOR_H

#include <QDialog>

namespace Ui {
class PaymentMethodEditor;
}

class PaymentMethodEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~PaymentMethodEditor();
    explicit PaymentMethodEditor(QString *name,
                                 QWidget *parent=nullptr);

public slots:
    void accept();

private:
    QString *_name;
    Ui::PaymentMethodEditor *ui;
};

#endif // PAYMENTMETHODEDITOR_H
