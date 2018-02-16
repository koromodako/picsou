#include "qcryptomodule.h"

#ifndef USE_WIN_CRYPTO_API
#   include <gcrypt.h>
#else
#   error   not implemented !
#endif

#include "utils/macro.h"

QCryptoModule::~QCryptoModule()
{

}

QCryptoModule::QCryptoModule() :
    QCryptoWrapper()
{

}

bool QCryptoModule::initialize(size_t sec_mem_pool_size)
{
    bool success=false;

    if(gcry_check_version(GCRYPT_VERSION)==nullptr) {
        goto end;
    }
    if(!wrap(gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN))) {
        goto end;
    }
    if(!wrap(gcry_control(GCRYCTL_INIT_SECMEM, sec_mem_pool_size, 0))) {
        goto end;
    }
    if(!wrap(gcry_control(GCRYCTL_RESUME_SECMEM_WARN))) {
        goto end;
    }
    if(!wrap(gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0))) {
        goto end;
    }

    success=true;
end:
    return success;
}

void QCryptoModule::terminate()
{

}
