#ifndef QPASSPHRASE_H
#define QPASSPHRASE_H

#include "qcryptowrapper.h"
#include "qsecurememory.h"

class QPassphrase : public QCryptoWrapper, public QSecureMemory
{
public:
    enum KDFAlgorithm {
        /*
         * The OpenPGP simple S2K algorithm (cf. RFC4880). Its use is strongly
         * deprecated. salt and iterations are not needed and may be passed as
         * nullptr/0.
         */
        //SIMPLE_S2K,
        /*
         * The OpenPGP salted S2K algorithm (cf. RFC4880). Usually not used.
         * iterations is not needed and may be passed as 0. saltlen must be
         * given as 8.
         */
        SALTED_S2K,
        /*
         * The OpenPGP iterated+salted S2K algorithm (cf. RFC4880). This is the
         * default for most OpenPGP applications. saltlen must be given as 8.
         * Note that OpenPGP defines a special encoding of the iterations;
         * however this function takes the plain decoded iteration count.
         */
        ITERSALTED_S2K,
        /*
         * The PKCS#5 Passphrase Based Key Derivation Function number 2.
         */
        PBKDF2,
        /*
         * The SCRYPT Key Derivation Function. The subalgorithm is used to
         * specify the CPU/memory cost parameter N, and the number of
         * iterations is used for the parallelization parameter p.
         * The block size is fixed at 8 in the current implementation.
         */
        SCRYPT
    };

    virtual ~QPassphrase();
    QPassphrase(size_t length);

    bool derived(KDFAlgorithm algo,
                 int subalgo,
                 const QSecureMemory salt,
                 QSecureMemory key,
                 ulong iterations);
};

#endif // QPASSPHRASE_H
