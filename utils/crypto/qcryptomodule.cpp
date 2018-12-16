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
#include "qcryptomodule.h"
#include "utils/macro.h"
#include <gcrypt.h>

QCryptoModule::~QCryptoModule()
{

}

QCryptoModule::QCryptoModule() :
    QCryptoWrapper()
{

}

bool QCryptoModule::initialize(size_t sec_mem_pool_size)
{
    LOG_IN("sec_mem_pool_size");
    if(gcry_check_version(GCRYPT_VERSION)==nullptr) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN))) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_control(GCRYCTL_INIT_SECMEM, sec_mem_pool_size, 0))) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_control(GCRYCTL_RESUME_SECMEM_WARN))) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0))) {
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

void QCryptoModule::terminate()
{

}
