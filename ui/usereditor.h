#ifndef USEREDITOR_H
#define USEREDITOR_H

#include <QWidget>

#include "picsouui.h"

namespace Ui {
class UserEditor;
}

class UserEditor : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~UserEditor();
    explicit UserEditor(PicsouUIService *ui_svc, QWidget *parent = 0);

private:
    Ui::UserEditor *ui;
};

#endif // USEREDITOR_H
