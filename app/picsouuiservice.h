#ifndef PICSOUUISERVICE_H
#define PICSOUUISERVICE_H

#include "picsouabstractservice.h"
#include "ui/mainwindow.h"

class PicsouUIService : public PicsouAbstractService
{
    Q_OBJECT
public:
    virtual ~PicsouUIService();
    explicit PicsouUIService(PicsouApplication *papp);

    bool initialize();
    void terminate();

signals:

public slots:

private:
    MainWindow _mw;
};

#endif // PICSOUUISERVICE_H
