#ifndef LOCKED_OBJECT_VIEWER_H
#define LOCKED_OBJECT_VIEWER_H

#include "ui/picsouuiviewer.h"

namespace Ui {
class LockedObjectViewer;
}

class LockedObjectViewer : public PicsouUIViewer
{
    Q_OBJECT

public:
    virtual ~LockedObjectViewer();
    explicit LockedObjectViewer(PicsouUIService *ui_svc,
                                QUuid uuid,
                                QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    void unlock();

private:
    Ui::LockedObjectViewer *ui;
};

#endif // LOCKED_OBJECT_VIEWER_H
