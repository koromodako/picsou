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
#include "cryptobuffer.h"
#include "utils/macro.h"
#include "third-party/build/include/gcrypt.h"

CryptoBuffer::~CryptoBuffer()
{
    if(m_buf!=nullptr) {
        gcry_free(m_buf);
    }
}

CryptoBuffer::CryptoBuffer(char *buf, int size) :
    m_buf(buf),
    m_size(size)
{

}

CryptoBufferShPtr CryptoBuffer::malloc(int size)
{
    char *buf=static_cast<char*>(gcry_malloc_secure(static_cast<size_t>(size)));
    if(buf==nullptr) {
        LOG_CRITICAL("failed to allocate secure memory.");
        return CryptoBufferShPtr();
    }
    return CryptoBufferShPtr(new CryptoBuffer(buf, size));
}

CryptoBufferShPtr CryptoBuffer::calloc(int mem_size, int mem_cnt)
{
    char *buf=static_cast<char*>(gcry_calloc_secure(static_cast<size_t>(mem_cnt),
                                                    static_cast<size_t>(mem_size)));
    if(buf==nullptr) {
        LOG_CRITICAL("failed to allocate secure memory.");
        return CryptoBufferShPtr();
    }
    return CryptoBufferShPtr(new CryptoBuffer(buf, mem_cnt*mem_size));
}

CryptoBufferShPtr CryptoBuffer::random(int size, RandomLevel lvl)
{
    gcry_random_level_t level;
    switch (lvl) {
    case RANDOM_WEAK:
        level=GCRY_WEAK_RANDOM;
        break;
    case RANDOM_STRONG:
        level=GCRY_STRONG_RANDOM;
        break;
    case RANDOM_VERY_STRONG:
        level=GCRY_VERY_STRONG_RANDOM;
        break;
    }
    char *buf=static_cast<char*>(gcry_random_bytes_secure(static_cast<size_t>(size), level));
    if(buf==nullptr) {
        LOG_CRITICAL("failed to allocate secure memory.");
        return CryptoBufferShPtr();
    }
    return CryptoBufferShPtr(new CryptoBuffer(buf, size));
}

QByteArray CryptoBuffer::random_ba(int size, CryptoBuffer::RandomLevel lvl)
{
    CryptoBufferShPtr rand=random(size, lvl);
    return QByteArray(rand->rbuf(), size);
}
