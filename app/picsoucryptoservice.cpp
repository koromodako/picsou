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
#include "picsoucryptoservice.h"
#include "utils/macro.h"

PicsouCryptoService::~PicsouCryptoService()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

PicsouCryptoService::PicsouCryptoService(PicsouApplication *papp) :
    PicsouAbstractService(papp),
    _crypto_mod()
{
    LOG_IN_VOID();
    LOG_VOID_RETURN();
}

bool PicsouCryptoService::initialize()
{
    LOG_IN_VOID();
    LOG_BOOL_RETURN(_crypto_mod.initialize());
}

void PicsouCryptoService::terminate()
{
    LOG_IN_VOID();
    _crypto_mod.terminate();
    LOG_VOID_RETURN();
}
