#ifndef PICSOUMODELSERVICE_H
#define PICSOUMODELSERVICE_H

#include "picsouabstractservice.h"

class PicsouModelService : public PicsouAbstractService
{
    Q_OBJECT
public:
    virtual ~PicsouModelService();
    explicit PicsouModelService(PicsouApplication *papp);

    bool initialize();
    void terminate();

signals:

public slots:

};

#endif // PICSOUMODELSERVICE_H
