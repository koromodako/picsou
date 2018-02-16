#ifndef BUDGETEDITOR_H
#define BUDGETEDITOR_H

#include <QWidget>

namespace Ui {
class BudgetEditor;
}

class BudgetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetEditor(QWidget *parent = 0);
    ~BudgetEditor();

private:
    Ui::BudgetEditor *ui;
};

#endif // BUDGETEDITOR_H
