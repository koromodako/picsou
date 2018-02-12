#include "qpassphrase.h"

#ifndef USE_WIN_CRYPTO_API
#   include <gcrypt.h>
#else
#   error   not implemented !
#endif

QPassphrase::~QPassphrase()
{

}

QPassphrase::QPassphrase(size_t length) :
    QSecureMemory(length)
{

}

bool QPassphrase::derived(KDFAlgorithm algo,
                          int subalgo,
                          QSecureMemory salt,
                          QSecureMemory key,
                          ulong iterations)
{
    gpg_error_t success=-1;
    int kdf_algo;

    if(!valid()) {
        goto end;
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

    success=gcry_kdf_derive(const_data(), length(),
                            kdf_algo, subalgo,
                            salt.const_data(), salt.length(),
                            iterations,
                            key.length(), key.data());
end:
    return (success==0);
}
