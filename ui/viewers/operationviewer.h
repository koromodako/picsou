#ifndef OPERATIONVIEWER_H
#define OPERATIONVIEWER_H

#include "ui/picsouuiviewer.h"
#include "ui/widgets/picsoutablewidget.h"

namespace Ui {
class OperationViewer;
}

class OperationViewer : public PicsouUIViewer
{
    Q_OBJECT
public:
    enum ViewerScale {
        VS_YEAR,
        VS_MONTH
    };

    virtual ~OperationViewer();
    explicit OperationViewer(PicsouUIService *ui_svc,
                             QUuid user_id,
                             QUuid account_id,
                             ViewerScale scale,
                             int year=-1,
                             int month=-1,
                             QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    /* ops */
    void add_op();
    void edit_op();
    void remove_op();
    void table_edit_op(int row, int col);

private:
    QUuid _user_id;
    int _year;
    int _month;
    ViewerScale _scale;
    PicsouTableWidget *_table;
    Ui::OperationViewer *ui;
};

#endif // OPERATIONVIEWER_H
