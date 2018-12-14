#include "qcryptomodule.h"
#include "utils/macro.h"
#include <gcrypt.h>

QCryptoModule::~QCryptoModule()
{

}

QCryptoModule::QCryptoModule() :
    QCryptoWrapper()
{

}

bool QCryptoModule::initialize(size_t sec_mem_pool_size)
{
    LOG_IN("sec_mem_pool_size");
    if(gcry_check_version(GCRYPT_VERSION)==nullptr) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN))) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_control(GCRYCTL_INIT_SECMEM, sec_mem_pool_size, 0))) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_control(GCRYCTL_RESUME_SECMEM_WARN))) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0))) {
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

void QCryptoModule::terminate()
{

}
