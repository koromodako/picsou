#ifndef QCRYPTOMODULE_H
#define QCRYPTOMODULE_H

#include "qcipher.h"
#include "qmessageauthenticationcode.h"
#include "qnonce.h"
#include "qpassphrase.h"
#include "qrandombytes.h"
#include "qsecurehash.h"
#include "qsecurememory.h"

class QCryptoModule : public QCryptoWrapper
{
public:
    virtual ~QCryptoModule();
    QCryptoModule();

    bool initialize(size_t sec_mem_pool_size=16384);
    void terminate();
};

#endif // QCRYPTOMODULE_H
