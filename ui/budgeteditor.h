#ifndef BUDGETEDITOR_H
#define BUDGETEDITOR_H

#include <QWidget>

#include "picsouui.h"

namespace Ui {
class BudgetEditor;
}

class BudgetEditor : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~BudgetEditor();
    explicit BudgetEditor(PicsouUIService *ui_svc, QWidget *parent = 0);

private:
    Ui::BudgetEditor *ui;
};

#endif // BUDGETEDITOR_H
