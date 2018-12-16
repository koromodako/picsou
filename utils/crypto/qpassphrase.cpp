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
#include "qpassphrase.h"
#include "utils/macro.h"
#include <gcrypt.h>

QPassphrase::~QPassphrase()
{

}

QPassphrase::QPassphrase(size_t length) :
    QCryptoWrapper(),
    QSecureMemory(length)
{

}

bool QPassphrase::derived(KDFAlgorithm algo,
                          int subalgo,
                          const QSecureMemory salt,
                          QSecureMemory key,
                          ulong iterations)
{
    int kdf_algo;
    if(!valid()) {
        LOG_BOOL_RETURN(false);
    }
    switch (algo) {
        /*case SIMPLE_S2K: gcry_algo=GCRY_KDF_SIMPLE_S2K; break;*/
        case SALTED_S2K:
            kdf_algo=GCRY_KDF_SALTED_S2K;
            break;
        case ITERSALTED_S2K:
            kdf_algo=GCRY_KDF_ITERSALTED_S2K;
            break;
        case PBKDF2:
            kdf_algo=GCRY_KDF_PBKDF2;
            break;
        case SCRYPT:
            kdf_algo=GCRY_KDF_SCRYPT;
            break;
    }

    LOG_BOOL_RETURN(wrap(gcry_kdf_derive(const_data(), length(),
                                 kdf_algo, subalgo,
                                 salt.const_data(), salt.length(),
                                 iterations,
                                 key.length(), key.data())));
}
