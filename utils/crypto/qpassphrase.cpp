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
