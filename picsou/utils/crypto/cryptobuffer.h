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
#ifndef CRYPTOBUFFER_H
#define CRYPTOBUFFER_H

#include <QString>
#include <QSharedPointer>

class CryptoBuffer;
typedef QSharedPointer<CryptoBuffer> CryptoBufferShPtr;

class CryptoBuffer
{
public:
    enum RandomLevel {
        RANDOM_WEAK,
        RANDOM_STRONG,
        RANDOM_VERY_STRONG
    };

    virtual ~CryptoBuffer();
    CryptoBuffer(char *buf=nullptr, int size=0);

    static CryptoBufferShPtr malloc(int size);
    static CryptoBufferShPtr calloc(int mem_size, int mem_cnt);
    static CryptoBufferShPtr random(int size, RandomLevel lvl);
    static QByteArray random_ba(int size, RandomLevel lvl);

    inline bool null() const { return m_buf==nullptr; }
    inline int size() const { return m_size; }

    inline char *rwbuf() { return m_buf; }
    inline const char *rbuf() const { return m_buf; }

protected:
    Q_DISABLE_COPY(CryptoBuffer)

private:
    char *m_buf;
    int m_size;
};

#endif // CRYPTOBUFFER_H
