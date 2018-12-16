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
#ifndef QSECUREMEMORY_H
#define QSECUREMEMORY_H

#include <QByteArray>

class QSecureMemory
{
public:
    virtual ~QSecureMemory();
    QSecureMemory(size_t length=0);
    QSecureMemory(const QByteArray ba);

    bool valid() const;
    bool empty() const;
    void erase();
    bool resize(size_t length);

    inline uchar *data() { return _sbuf; }
    inline const uchar *const_data() const { return _sbuf; }
    inline size_t length() const { return _length; }

private:
    uchar *_sbuf;
    size_t _length;
};

#endif // QSECUREMEMORY_H
