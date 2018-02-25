#ifndef USERVIEWER_H
#define USERVIEWER_H

#include "ui/picsouuiviewer.h"

#include "model/picsoudb.h"

namespace Ui {
class UserViewer;
}

class UserViewer : public PicsouUIViewer
{
    Q_OBJECT

public:
    virtual ~UserViewer();
    explicit UserViewer(PicsouUIService *ui_svc,
                        QUuid uuid,
                        QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    void add_account();
    void edit_account();
    void remove_account();

    void add_budget();
    void edit_budget();
    void remove_budget();

private:
    Ui::UserViewer *ui;
};

#endif // USERVIEWER_H
