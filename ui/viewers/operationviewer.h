#ifndef OPERATIONVIEWER_H
#define OPERATIONVIEWER_H

#include "ui/picsouuiviewer.h"

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

    explicit OperationViewer(PicsouUIService *ui_svc,
                             QUuid account_id,
                             ViewerScale scale,
                             int ym,
                             QWidget *parent = 0);
    ~OperationViewer();

public slots:
    void refresh(const PicsouDBPtr db);

    /* ops */
    void add_op();
    void edit_op();
    void remove_op();

private:
    int _ym;
    ViewerScale _scale;
    Ui::OperationViewer *ui;
};

#endif // OPERATIONVIEWER_H
