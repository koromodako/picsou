#ifndef PICSOUDBVIEWER_H
#define PICSOUDBVIEWER_H

#include <QWidget>

#include "picsouui.h"

namespace Ui {
class PicsouDBViewer;
}

class PicsouDBViewer : public QWidget, public PicsouUI
{
    Q_OBJECT

public:
    virtual ~PicsouDBViewer();
    explicit PicsouDBViewer(PicsouUIService *ui_svc, QWidget *parent = 0);

private:
    Ui::PicsouDBViewer *ui;
};

#endif // PICSOUDBVIEWER_H
