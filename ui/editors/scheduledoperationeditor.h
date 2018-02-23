#ifndef SCHEDULEDOPERATIONEDITOR_H
#define SCHEDULEDOPERATIONEDITOR_H

#include <QWidget>

#include "ui/picsouui.h"

namespace Ui {
class ScheduledOperationEditor;
}

class ScheduledOperationEditor : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~ScheduledOperationEditor();
    explicit ScheduledOperationEditor(PicsouUIService *ui_svc, QWidget *parent = 0);

private:
    Ui::ScheduledOperationEditor *ui;
};

#endif // SCHEDULEDOPERATIONEDITOR_H
