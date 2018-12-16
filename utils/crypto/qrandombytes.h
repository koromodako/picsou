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
#ifndef QRANDOMBYTES_H
#define QRANDOMBYTES_H

#include "qsecurememory.h"

class QRandomBytes : public QSecureMemory
{
public:
    enum RandomnessLevel {
        /*
         * For all functions, except for gcry_mpi_randomize, this level maps
         * to GCRY_STRONG_RANDOM. If you do not want this, consider using
         * gcry_create_nonce.
         */
        WEAK_RANDOM,
        /*
         * Use this level for session keys and similar purposes.
         */
        STRONG_RANDOM,
        /*
         * Use this level for long term key material.
         */
        VERY_STRONG_RANDOM
    };

    virtual ~QRandomBytes();
    QRandomBytes(size_t length, RandomnessLevel level);
};

#endif // QRANDOMBYTES_H
