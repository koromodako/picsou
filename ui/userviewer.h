#ifndef USERVIEWER_H
#define USERVIEWER_H

#include <QWidget>

#include"picsouui.h"

namespace Ui {
class UserViewer;
}

class UserViewer : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~UserViewer();
    explicit UserViewer(PicsouUIService * ui_svc, QWidget *parent = 0);

private:
    Ui::UserViewer *ui;
};

#endif // USERVIEWER_H
