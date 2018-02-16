#ifndef OPERATIONEDITOR_H
#define OPERATIONEDITOR_H

#include <QWidget>

#include "picsouui.h"

namespace Ui {
class OperationEditor;
}

class OperationEditor : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~OperationEditor();
    explicit OperationEditor(PicsouUIService *ui_svc, QWidget *parent = 0);

private:
    Ui::OperationEditor *ui;
};

#endif // OPERATIONEDITOR_H
