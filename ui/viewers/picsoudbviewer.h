#ifndef PICSOUDBVIEWER_H
#define PICSOUDBVIEWER_H

#include <QUuid>
#include <QWidget>

#include "model/picsoudb.h"

#include "ui/picsouuiviewer.h"

namespace Ui {
class PicsouDBViewer;
}

class PicsouDBViewer : public PicsouUIViewer
{
    Q_OBJECT
public:
    virtual ~PicsouDBViewer();
    explicit PicsouDBViewer(PicsouUIService *ui_svc,
                            QUuid uuid,
                            QWidget *parent=nullptr);

public slots:
    void refresh(const PicsouDBPtr db);

    void add_user();
    void edit_user();
    void remove_user();

private:
    Ui::PicsouDBViewer *ui;
};

#endif // PICSOUDBVIEWER_H
