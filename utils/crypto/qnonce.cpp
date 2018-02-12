#include "qnonce.h"

#ifndef USE_WIN_CRYPTO_API
#   include <gcrypt.h>
#else
#   error   not implemented !
#endif

QNonce::~QNonce()
{

}

QNonce::QNonce(size_t length) :
    QSecureMemory(length)
{
    gcry_create_nonce(data(), this->length());
}
