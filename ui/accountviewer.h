#ifndef ACCOUNTVIEWER_H
#define ACCOUNTVIEWER_H

#include <QWidget>

#include "picsouui.h"

namespace Ui {
class AccountViewer;
}

class AccountViewer : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~AccountViewer();
    explicit AccountViewer(PicsouUIService *ui_svc, QWidget *parent = 0);

private:
    Ui::AccountViewer *ui;
};

#endif // ACCOUNTVIEWER_H
