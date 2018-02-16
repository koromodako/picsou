#ifndef SCHEDULEDOPERATIONEDITOR_H
#define SCHEDULEDOPERATIONEDITOR_H

#include <QWidget>

namespace Ui {
class ScheduledOperationEditor;
}

class ScheduledOperationEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ScheduledOperationEditor(QWidget *parent = 0);
    ~ScheduledOperationEditor();

private:
    Ui::ScheduledOperationEditor *ui;
};

#endif // SCHEDULEDOPERATIONEDITOR_H
