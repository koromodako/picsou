#ifndef QMESSAGEAUTHCODE_H
#define QMESSAGEAUTHCODE_H

#include "qcryptowrapper.h"
#include "qsecurememory.h"

#ifndef USE_WIN_CRYPTO_API
typedef struct gcry_mac_handle *gcry_mac_hd_t;
#else
#   error   not implemented !
#endif

class QMessageAuthCode : public QCryptoWrapper
{
public:
    enum MACAlgorithm {
        /*
         *  This is keyed-hash message authentication code (HMAC) message
         *  authentication algorithm based on the SHA-256 hash algorithm.
         */
        HMAC_SHA256,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  SHA-224 hash algorithm.
         */
        HMAC_SHA224,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  SHA-512 hash algorithm.
         */
        HMAC_SHA512,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  SHA-384 hash algorithm.
         */
        HMAC_SHA384,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  SHA3-384 hash algorithm.
         */
        HMAC_SHA3_256,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  SHA3-224 hash algorithm.
         */
        HMAC_SHA3_224,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  SHA3-512 hash algorithm.
         */
        HMAC_SHA3_512,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  SHA3-384 hash algorithm.
         */
        HMAC_SHA3_384,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  SHA-1 hash algorithm.
         */
        HMAC_SHA1,
        /*
         *  This is HMAC message authentication algorithm based on the MD5
         *  hash algorithm.
         */
        HMAC_MD5,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  MD4 hash algorithm.
         */
        HMAC_MD4,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  RIPE-MD-160 hash algorithm.
         */
        HMAC_RMD160,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  WHIRLPOOL hash algorithm.
         */
        HMAC_WHIRLPOOL,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  GOST R 34.11-94 hash algorithm.
         */
        HMAC_GOSTR3411_94,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  256-bit hash algorithm described in GOST R 34.11-2012.
         */
        HMAC_STRIBOG256,
        /*
         *  This is HMAC message authentication algorithm based on the
         *  512-bit hash algorithm described in GOST R 34.11-2012.
         */
        HMAC_STRIBOG512,
        /*
         *  This is CMAC (Cipher-based MAC) message authentication algorithm
         *  based on the AES block cipher algorithm.
         */
        CMAC_AES,
        /*
         *  This is CMAC message authentication algorithm based on the
         *  three-key EDE Triple-DES block cipher algorithm.
         */
        CMAC_3DES,
        /*
         *  This is CMAC message authentication algorithm based on the
         *  Camellia block cipher algorithm.
         */
        CMAC_CAMELLIA,
        /*
         *  This is CMAC message authentication algorithm based on the
         *  CAST128-5 block cipher algorithm.
         */
        CMAC_CAST5,
        /*
         *  This is CMAC message authentication algorithm based on the
         *  Blowfish block cipher algorithm.
         */
        CMAC_BLOWFISH,
        /*
         *  This is CMAC message authentication algorithm based on the
         *  Twofish block cipher algorithm.
         */
        CMAC_TWOFISH,
        /*
         *  This is CMAC message authentication algorithm based on the
         *  Serpent block cipher algorithm.
         */
        CMAC_SERPENT,
        /*
         *  This is CMAC message authentication algorithm based on the
         *  SEED block cipher algorithm.
         */
        CMAC_SEED,
        /*
         *  This is CMAC message authentication algorithm based on the Ron’s
         *  Cipher 2 block cipher algorithm.
         */
        CMAC_RFC2268,
        /*
         *  This is CMAC message authentication algorithm based on the IDEA
         *  block cipher algorithm.
         */
        CMAC_IDEA,
        /*
         *  This is CMAC message authentication algorithm based on the
         *  GOST 28147-89 block cipher algorithm.
         */
        CMAC_GOST28147,
        /*
         *  This is GMAC (GCM mode based MAC) message authentication algorithm
         *  based on the AES block cipher algorithm.
         */
        GMAC_AES,
        /*
         *  This is GMAC message authentication algorithm based on the Camellia
         *  block cipher algorithm.
         */
        GMAC_CAMELLIA,
        /*
         *  This is GMAC message authentication algorithm based on the Twofish
         *  block cipher algorithm.
         */
        GMAC_TWOFISH,
        /*
         *  This is GMAC message authentication algorithm based on the Serpent
         *  block cipher algorithm.
         */
        GMAC_SERPENT,
        /*
         *  This is GMAC message authentication algorithm based on the SEED
         *  block cipher algorithm.
         */
        GMAC_SEED,
        /*
         *  This is plain Poly1305 message authentication algorithm, used
         *  with one-time key.
         */
        POLY1305,
        /*
         *  This is Poly1305-AES message authentication algorithm, used with
         *  key and one-time nonce.
         */
        POLY1305_AES,
        /*
         *  This is Poly1305-Camellia message authentication algorithm, used
         *  with key and one-time nonce.
         */
        POLY1305_CAMELLIA,
        /*
         *  This is Poly1305-Twofish message authentication algorithm, used
         *  with key and one-time nonce.
         */
        POLY1305_TWOFISH,
        /*
         *  This is Poly1305-Serpent message authentication algorithm, used
         *  with key and one-time nonce.
         */
        POLY1305_SERPENT,
        /*
         *  This is Poly1305-SEED message authentication algorithm, used
         *  with key and one-time nonce.
         */
        POLY1305_SEED
    };

    enum MACFlag {
        NONE = 0x00,
        /*
         * Allocate all buffers and the resulting MAC in "secure memory". Use
         * this if the MAC data is highly confidential.
         */
        SECURE = 0x01
    };

    virtual ~QMessageAuthCode();
    QMessageAuthCode(MACAlgorithm algo,
                     MACFlag flags,
                     const QSecureMemory key,
                     const QSecureMemory iv);

    bool valid();
    bool reset(const QSecureMemory iv);
    bool update(const QSecureMemory data);
    bool verify(const QSecureMemory mac);
    bool digest(QSecureMemory mac);

private:
#ifndef USE_WIN_CRYPTO_API
    gcry_mac_hd_t _hd;
#else
#   error   not implemented !
#endif
};

#endif // QMESSAGEAUTHCODE_H
