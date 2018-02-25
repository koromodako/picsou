#ifndef QSECUREHASH_H
#define QSECUREHASH_H

#include "qcryptowrapper.h"
#include "qsecurememory.h"

#ifndef USE_WIN_CRYPTO_API
typedef struct gcry_md_handle *gcry_md_hd_t;
#else
#   error   not implemented !
#endif

class QSecureHash : public QCryptoWrapper
{
public:
    enum HashAlgorithm {
        /*
         *  This is the SHA-1 algorithm which yields a message digest of 20
         *  bytes. Note that SHA-1 begins to show some weaknesses and it is
         *  suggested to fade out its use if strong cryptographic properties
         *  are required.
         */
        SHA1,
        /*
         *  This is the 160 bit version of the RIPE message digest
         *  (RIPE-MD-160). Like SHA-1 it also yields a digest of 20 bytes. This
         *  algorithm share a lot of design properties with SHA-1 and thus it
         *  is advisable not to use it for new protocols.
         */
        RMD160,
        /*
         *  This is the well known MD5 algorithm, which yields a message digest
         *  of 16 bytes. Note that the MD5 algorithm has severe weaknesses, for
         *  example it is easy to compute two messages yielding the same hash
         *  (collision attack). The use of this algorithm is only justified for
         *  non-cryptographic application.
         */
        MD5,
        /*
         *  This is the MD4 algorithm, which yields a message digest of 16
         *  bytes. This algorithm has severe weaknesses and should not be used.
         */
        MD4,
        /*
         *  This is an reserved identifier for MD-2; there is no implementation
         *  yet. This algorithm has severe weaknesses and should not be used.
         */
        MD2,
        /*
         *  This is the TIGER/192 algorithm which yields a message digest of 24
         *  bytes. Actually this is a variant of TIGER with a different output
         *  print order as used by GnuPG up to version 1.3.2.
         */
        TIGER,
        /*
         *  This is the TIGER variant as used by the NESSIE project. It uses the
         *  most commonly used output print order.
         */
        TIGER1,
        /*
         *  This is another variant of TIGER with a different padding scheme.
         */
        TIGER2,
        /*  This is an reserved value for the HAVAL algorithm with 5 passes and
         *  160 bit. It yields a message digest of 20 bytes. Note that there is
         *  no implementation yet available.
         */
        HAVAL,
        /*
         *  This is the SHA-224 algorithm which yields a message digest of
         *  28 bytes. See Change Notice 1 for FIPS 180-2 for the specification.
         */
        SHA224,
        /*
         *  This is the SHA-256 algorithm which yields a message digest of 32
         *  bytes. See FIPS 180-2 for the specification.
         */
        SHA256,
        /*
         *  This is the SHA-384 algorithm which yields a message digest of 48
         *  bytes. See FIPS 180-2 for the specification.
         */
        SHA384,
        /*
         *  This is the SHA-384 algorithm which yields a message digest of 64
         *  bytes. See FIPS 180-2 for the specification.
         */
        SHA512,
        /*
         *  This is the SHA3-224 algorithm which yields a message digest of 28
         *  bytes. See FIPS 202 for the specification.
         */
        SHA3_224,
        /*
         *  This is the SHA3-256 algorithm which yields a message digest of 32
         *  bytes. See FIPS 202 for the specification.
         */
        SHA3_256,
        /*
         *  This is the SHA3-384 algorithm which yields a message digest of 48
         *  bytes. See FIPS 202 for the specification.
         */
        SHA3_384,
        /*
         *  This is the SHA3-384 algorithm which yields a message digest of 64
         *  bytes. See FIPS 202 for the specification.
         */
        SHA3_512,
        /*
         *  This is the SHAKE128 extendable-output function (XOF) algorithm
         *  with 128 bit security strength. See FIPS 202 for the specification.
         */
        SHAKE128,
        /*
         *  This is the SHAKE256 extendable-output function (XOF) algorithm
         *  with 256 bit security strength. See FIPS 202 for the specification.
         */
        SHAKE256,
        /*
         *  This is the ISO 3309 and ITU-T V.42 cyclic redundancy check. It
         *  yields an output of 4 bytes. Note that this is not a hash
         *  algorithm in the cryptographic sense.
         */
        CRC32,
        /*
         *  This is the above cyclic redundancy check function, as modified by
         *  RFC 1510. It yields an output of 4 bytes. Note that this is not a
         *  hash algorithm in the cryptographic sense.
         */
        CRC32_RFC1510,
        /*
         *  This is the OpenPGP cyclic redundancy check function. It yields an
         *  output of 3 bytes. Note that this is not a hash algorithm in the
         *  cryptographic sense.
         */
        CRC24_RFC2440,
        /*
         *  This is the Whirlpool algorithm which yields a message digest of 64
         *  bytes.
         */
        WHIRLPOOL,
        /*
         *  This is the hash algorithm described in GOST R 34.11-94 which
         *  yields a message digest of 32 bytes.
         */
        GOSTR3411_94,
        /*
         *  This is the 256-bit version of hash algorithm described in GOST R
         *  34.11-2012 which yields a message digest of 32 bytes.
         */
        STRIBOG256,
        /*  This is the 512-bit version of hash algorithm described in GOST R
         *  34.11-2012 which yields a message digest of 64 bytes.
         */
        STRIBOG512
    };

    enum HashFlag {
        /*
         *  Allocate all buffers and the resulting digest in "secure memory".
         *  Use this is the hashed data is highly confidential.
         */
        SECURE,
        /*
         *  Turn the algorithm into a HMAC message authentication algorithm.
         *  This only works if just one algorithm is enabled for the handle and
         *  that algorithm is not an extendable-output function. Note that the
         *  function gcry_md_setkey must be used to set the MAC key. The size of
         *  the MAC is equal to the message digest of the underlying hash
         *  algorithm. If you want CBC message authentication codes based on a
         *  cipher, see See Working with cipher handles.
         */
        HMAC,
        /*
         *  Versions of Libgcrypt before 1.6.0 had a bug in the Whirlpool code
         *  which led to a wrong result for certain input sizes and write
         *  patterns. Using this flag emulates that bug. This may for example
         *  be useful for applications which use Whirlpool as part of their key
         *  generation. It is strongly suggested to use this flag only if
         *  really needed and if possible to the data should be re-processed
         *  using the regular Whirlpool algorithm.
         */
        BUGEMU1
    };

    virtual ~QSecureHash();
    QSecureHash(HashAlgorithm algo,
                HashFlag flags,
                const QSecureMemory key=QSecureMemory());

    bool valid() const;
    void reset();
    void update(const QSecureMemory data);
    bool digest(QSecureMemory digest);

private:
#ifndef USE_WIN_CRYPTO_API
    int _algo;
    gcry_md_hd_t _hd;
#else
#   error   not implemented !
#endif
};

#endif // QSECUREHASH_H
