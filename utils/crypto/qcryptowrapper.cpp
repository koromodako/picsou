#include "qcryptowrapper.h"
#include "utils/macro.h"
#include <gcrypt.h>

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
    if(err!=GPG_ERR_NO_ERROR) {
        _latest_error_source=QString(gcry_strsource(err));
        _latest_error_description=QString(gcry_strerror(err));
        return false;
    }
    return true;
}
