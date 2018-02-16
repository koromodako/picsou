#ifndef OPERATIONEDITOR_H
#define OPERATIONEDITOR_H

#include <QWidget>

namespace Ui {
class OperationEditor;
}

class OperationEditor : public QWidget
{
    Q_OBJECT

public:
    explicit OperationEditor(QWidget *parent = 0);
    ~OperationEditor();

private:
    Ui::OperationEditor *ui;
};

#endif // OPERATIONEDITOR_H
