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
