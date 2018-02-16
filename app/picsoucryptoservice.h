#ifndef PICSOUCRYPTOSERVICE_H
#define PICSOUCRYPTOSERVICE_H

#include "picsouabstractservice.h"
#include "utils/crypto/qcryptomodule.h"

class PicsouCryptoService : public PicsouAbstractService
{
    Q_OBJECT
public:
    virtual ~PicsouCryptoService();
    explicit PicsouCryptoService(PicsouApplication *papp);

    bool initialize();
    void terminate();

signals:

public slots:

private:
    QCryptoModule _crypto_mod;
};

#endif // PICSOUCRYPTOSERVICE_H
