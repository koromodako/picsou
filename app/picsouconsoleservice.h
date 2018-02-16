#ifndef PICSOUCONSOLESERVICE_H
#define PICSOUCONSOLESERVICE_H

#include "picsouabstractservice.h"

class PicsouConsoleService : public PicsouAbstractService
{
    Q_OBJECT
public:
    virtual ~PicsouConsoleService();
    explicit PicsouConsoleService(PicsouApplication *papp);

    bool initialize();
    void terminate();

signals:

public slots:
};

#endif // PICSOUCONSOLESERVICE_H
