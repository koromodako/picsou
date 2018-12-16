/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "qsecurememory.h"
#include "utils/macro.h"
#include <gcrypt.h>

#define FREE(buf) gcry_free((buf)); (buf)=nullptr

QSecureMemory::~QSecureMemory()
{
    erase();
    FREE(_sbuf);
    _length=0;
}

QSecureMemory::QSecureMemory(size_t length) :
    _sbuf(nullptr),
    _length(length)
{
    if(_length>0) {
        _sbuf=static_cast<uchar*>(gcry_malloc_secure(_length));
        if(_sbuf==nullptr) {
            _length=0; // malloc failed
        }
    }
}

QSecureMemory::QSecureMemory(const QByteArray ba) :
    _sbuf(nullptr),
    _length(static_cast<size_t>(ba.length()))
{
    if(_length>0) {
        _sbuf=static_cast<uchar*>(gcry_malloc_secure(_length));
        if(_sbuf==nullptr) {
            _length=0; // malloc failed
            return;
        }
        memcpy(_sbuf, ba.constData(), _length);
    }
    return;
}

bool QSecureMemory::valid() const
{
    return ((_length==0&&_sbuf!=nullptr)||(_length>0&&_sbuf==nullptr));
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
        if(_sbuf!=nullptr) {
            nbuf=static_cast<uchar*>(gcry_realloc(_sbuf, length));
        } else {
            nbuf=static_cast<uchar*>(gcry_malloc_secure(length));
        }

        success=(nbuf!=nullptr);

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
