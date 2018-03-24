#ifndef ACCOUNTVIEWER_H
#define ACCOUNTVIEWER_H

#include "ui/picsouuiviewer.h"
#include "ui/widgets/picsoutablewidget.h"

namespace Ui {
class AccountViewer;
}

class AccountViewer : public PicsouUIViewer
{
    Q_OBJECT

public:
    virtual ~AccountViewer();
    explicit AccountViewer(PicsouUIService *ui_svc,
                           QUuid user_uuid,
                           QUuid account_uuid,
                           QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    /* payment methods */
    void add_pm();
    void edit_pm();
    void remove_pm();
    /* scheduled ops */
    // TODO
    /* ops */
    void add_op();
    void edit_op();
    void remove_op();
    void import_ops();
    void export_ops();

    void table_edit_op(int row, int col);

private:
    QUuid _user_id;
    PicsouTableWidget *_table;
    Ui::AccountViewer *ui;
};

#endif // ACCOUNTVIEWER_H
