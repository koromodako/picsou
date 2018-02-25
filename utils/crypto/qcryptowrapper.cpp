#include "qcryptowrapper.h"

#ifndef USE_WIN_CRYPTO_API
#   include <gcrypt.h>
#else
#   error   not implemented !
#endif

QCryptoWrapper::~QCryptoWrapper()
{

}

QCryptoWrapper::QCryptoWrapper() :
    _latest_error_source(),
    _latest_error_description()
{

}

bool QCryptoWrapper::wrap(gcry_error_t err)
{
    bool success=true;
    if(err!=GPG_ERR_NO_ERROR) {
        _latest_error_source=QString(gcry_strsource(err));
        _latest_error_description=QString(gcry_strerror(err));
        success=false;
    }
    return success;
}
