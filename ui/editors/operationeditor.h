#ifndef OPERATIONEDITOR_H
#define OPERATIONEDITOR_H

#include <QDialog>

namespace Ui {
class OperationEditor;
}

class OperationEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~OperationEditor();
    explicit OperationEditor(double *amount,
                             QDate *date,
                             QString *payment_method,
                             QString *budget,
                             QString *recipient,
                             QString *description,
                             QWidget *parent = 0);

    inline void set_budgets(const QStringList &budgets);
    inline void set_payment_methods(const QStringList &payment_methods);

public slots:
    void accept();

private:
    double *_amount;
    QDate *_date;
    QString *_payment_method;
    QString *_budget;
    QString *_recipient;
    QString *_description;
    Ui::OperationEditor *ui;
};

#endif // OPERATIONEDITOR_H
