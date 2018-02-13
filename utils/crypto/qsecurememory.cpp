#include "qsecurememory.h"

#ifndef USE_WIN_CRYPTO_API
#   include <gcrypt.h>
#else
#   error   not implemented !
#endif

QSecureMemory::~QSecureMemory()
{
    erase();
    gcry_free(_sbuf);

    _sbuf=NULL;
    _length=-1;
}

QSecureMemory::QSecureMemory() :
    _sbuf(NULL),
    _length(-1)
{

}

QSecureMemory::QSecureMemory(size_t length)
{
    _sbuf=NULL;
    _length=-1;

    _sbuf=static_cast<uchar*>(gcry_malloc_secure(length));
    if(_sbuf!=NULL) {
        _length=length;
    }
}

QSecureMemory::QSecureMemory(uchar *sbuf, size_t length)
{
    _sbuf=NULL;
    _length=-1;

    if(length>0) {
        _sbuf=static_cast<uchar*>(gcry_malloc_secure(length));
        if(_sbuf!=NULL) {
            _length=length;
            memcpy(_sbuf, sbuf, _length);
        }
    }
}

bool QSecureMemory::valid() const
{
    return ((_sbuf!=NULL)&&(_length>0));
}

void QSecureMemory::erase()
{
    if(valid()) {
        memset(_sbuf, 0x00, _length);
    }
}

bool QSecureMemory::resize(size_t length)
{
    bool success=false;
    if(valid()) {
        uchar *nbuf=static_cast<uchar*>(gcry_realloc(_sbuf, length));
        success=(nbuf!=NULL);
        if(success) {
            _sbuf = nbuf;
            _length = length;
        }
    }
    return success;
}
