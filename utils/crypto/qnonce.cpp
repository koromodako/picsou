#include "qnonce.h"
#include "utils/macro.h"
#include <gcrypt.h>

QNonce::~QNonce()
{

}

QNonce::QNonce(size_t length) :
    QSecureMemory(length)
{
    if(valid()) {
        gcry_create_nonce(data(), this->length());
    }
}
