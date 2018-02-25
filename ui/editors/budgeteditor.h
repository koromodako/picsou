#ifndef BUDGETEDITOR_H
#define BUDGETEDITOR_H

#include <QDialog>

namespace Ui {
class BudgetEditor;
}

class BudgetEditor : public QDialog
{
    Q_OBJECT

public:
    virtual ~BudgetEditor();
    explicit BudgetEditor(double *amount,
                          QString *name,
                          QString *description,
                          QWidget *parent=nullptr);

public slots:
    void accept();

private:
    double *_amount;
    QString *_name;
    QString *_description;
    Ui::BudgetEditor *ui;
};

#endif // BUDGETEDITOR_H
