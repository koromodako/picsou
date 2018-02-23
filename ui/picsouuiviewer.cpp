#include "picsouuiviewer.h"

PicsouUIViewer::~PicsouUIViewer()
{

}

PicsouUIViewer::PicsouUIViewer(PicsouUIService *ui_svc,
                               QUuid uuid, QWidget *parent) :
    QWidget(parent),
    PicsouUI(ui_svc),
    _uuid(uuid)
{

}
