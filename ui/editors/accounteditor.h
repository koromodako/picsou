#ifndef ACCOUNTEDITOR_H
#define ACCOUNTEDITOR_H

#include <QWidget>

#include "ui/picsouui.h"

namespace Ui {
class AccountEditor;
}

class AccountEditor : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~AccountEditor();
    explicit AccountEditor(PicsouUIService *ui_svc, QWidget *parent = 0);

private:
    Ui::AccountEditor *ui;
};

#endif // ACCOUNTEDITOR_H
