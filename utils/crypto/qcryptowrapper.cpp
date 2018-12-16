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
