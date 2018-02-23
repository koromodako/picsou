#ifndef PICSOUUIVIEWER_H
#define PICSOUUIVIEWER_H

#include <QUuid>
#include <QWidget>

#include "picsouui.h"

#include "model/picsoudb.h"

class PicsouModelService;

class PicsouUIViewer : public QWidget, public PicsouUI
{
public:
    virtual ~PicsouUIViewer();

public slots:
    virtual void refresh(const PicsouDBPtr db) = 0;

protected:
    PicsouUIViewer(PicsouUIService *ui_svc,
                   QUuid uuid, QWidget *parent=nullptr);

    inline QUuid mod_obj_id() const { return _uuid; }

private:
    QUuid _uuid;
};

#endif // PICSOUUIVIEWER_H
