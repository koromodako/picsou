#include "qsecurememory.h"

#ifndef USE_WIN_CRYPTO_API
#   include <gcrypt.h>
#else
#   error   not implemented !
#endif

#define FREE(buf) gcry_free((buf)); (buf)=NULL

QSecureMemory::~QSecureMemory()
{
    erase();
    FREE(_sbuf);
    _length=-1;
}

QSecureMemory::QSecureMemory(size_t length) :
    _sbuf(NULL),
    _length(length)
{
    if(_length>0) {
        _sbuf=static_cast<uchar*>(gcry_malloc_secure(_length));
        if(_sbuf==NULL) {
            _length=-1; // malloc failed
        }
    }
}

QSecureMemory::QSecureMemory(uchar *sbuf, size_t length) :
    _sbuf(NULL),
    _length(length)
{
    if(_length>0) {
        _sbuf=static_cast<uchar*>(gcry_malloc_secure(length));
        if(_sbuf==NULL) {
            _length=-1; // malloc failed
            goto end;
        }
        memcpy(_sbuf, sbuf, _length);
    }
end:
    return;
}

bool QSecureMemory::valid() const
{
    return ((_length==0&&_sbuf!=NULL)||(_length>0&&_sbuf==NULL));
}

bool QSecureMemory::empty() const
{
    return (_length==0);
}

void QSecureMemory::erase()
{
    if(_length>0) {
        memset(_sbuf, 0x00, _length);
    }
}

bool QSecureMemory::resize(size_t length)
{
    uchar *nbuf;
    bool success=false;

    if(length>0) {
        if(_sbuf!=NULL) {
            nbuf=static_cast<uchar*>(gcry_realloc(_sbuf, length));
        } else {
            nbuf=static_cast<uchar*>(gcry_malloc_secure(length));
        }

        success=(nbuf!=NULL);

        if(success) {
            _sbuf=nbuf;
            _length=length;
        }
    } else if(length==0) {
        erase();
        FREE(_sbuf);
        _length=0;
    }

    return success;
}
