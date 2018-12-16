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
#ifndef QCRYPTOWRAPPER_H
#define QCRYPTOWRAPPER_H

#include <QString>

#ifndef USE_WIN_CRYPTO_API
typedef uint gcry_error_t;
#else
#   error   not implemented !
#endif

class QCryptoWrapper
{
public:
    virtual ~QCryptoWrapper();
    QCryptoWrapper();

    bool wrap(gcry_error_t err);

    inline QString err_source() { return _latest_error_source; }
    inline QString err_description() { return _latest_error_description; }

private:
    QString _latest_error_source;
    QString _latest_error_description;
};

#endif // QCRYPTOWRAPPER_H
