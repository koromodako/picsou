#ifndef PICSOUUISERVICE_H
#define PICSOUUISERVICE_H

#include "picsouabstractservice.h"

class MainWindow;

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
    void show_mainwindow();

private:
    MainWindow *_mw;
};

#endif // PICSOUUISERVICE_H
