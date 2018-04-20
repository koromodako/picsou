#ifndef OPERATIONEDITOR_H
#define OPERATIONEDITOR_H

#include <QDialog>
#include "model/amount.h"

namespace Ui {
class OperationEditor;
}

class OperationEditor : public QDialog
{
    Q_OBJECT
public:
    virtual ~OperationEditor();
    explicit OperationEditor(QDate *date,
                             Amount *amount,
                             QString *payment_method,
                             QString *budget,
                             QString *recipient,
                             QString *description,
                             int year=-1,
                             int month=-1,
                             QWidget *parent=nullptr);

    void set_budgets(const QStringList &budgets);
    void set_payment_methods(const QStringList &payment_methods);

public slots:
    void accept();

private:
    QDate *_date;
    Amount *_amount;
    QString *_payment_method;
    QString *_budget;
    QString *_recipient;
    QString *_description;
    int _year;
    int _month;
    Ui::OperationEditor *ui;
};

#endif // OPERATIONEDITOR_H
