#ifndef ACCOUNTVIEWER_H
#define ACCOUNTVIEWER_H

#include "ui/picsouuiviewer.h"

namespace Ui {
class AccountViewer;
}

class AccountViewer : public PicsouUIViewer
{
    Q_OBJECT

public:
    virtual ~AccountViewer();
    explicit AccountViewer(PicsouUIService *ui_svc,
                           QUuid uuid,
                           QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    /* payment methods */
    void add_pm();
    void edit_pm();
    void remove_pm();
    /* scheduled ops */
    // TODO

private:
    Ui::AccountViewer *ui;
};

#endif // ACCOUNTVIEWER_H
