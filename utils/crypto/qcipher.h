#ifndef QCIPHER_H
#define QCIPHER_H

#include "qsecurememory.h"

#ifndef USE_WIN_CRYPTO_API
typedef struct gcry_cipher_handle *gcry_cipher_hd_t;
#else
#   error   not implemented !
#endif

class QCipher
{
public:
    enum CipherAlgorithm {
        /*
         *  This is the IDEA algorithm.
         */
        IDEA,
        /*
         *  Triple-DES with 3 Keys as EDE. The key size of this algorithm is
         *  168 but you have to pass 192 bits because the most significant
         *  bits of each byte are ignored.
         */
        _3DES,
        /*
         *  CAST128-5 block cipher algorithm. The key size is 128 bits.
         */
        CAST5,
        /*
         *  The blowfish algorithm. The current implementation allows only for
         *  a key size of 128 bits.
         */
        BLOWFISH,
        /*
         *  Reserved and not currently implemented.
         */
        SAFER_SK128,
        /*
         *  Reserved and not currently implemented.
         */
        DES_SK,
        /*
         * AES (Rijndael) with a 128, 192 or 256 bit key.
         */
        AES,
        AES192,
        AES256,
        /*
         *  The Twofish algorithm with a 256 or 128 bit key.
         */
        TWOFISH,
        TWOFISH128,
        /*
         *  An algorithm which is 100% compatible with RSA Inc.’s RC4 algorithm.
         *  Note that this is a stream cipher and must be used very carefully
         *  to avoid a couple of weaknesses.
         */
        ARCFOUR,
        /*
         *  Standard DES with a 56 bit key. You need to pass 64 bit but the
         *  high bits of each byte are ignored. Note, that this is a weak
         *  algorithm which can be broken in reasonable time using a brute
         *  force approach.
         */
        DES,
        /*
         *  The Serpent cipher from the AES contest.
         */
        SERPENT128,
        SERPENT192,
        SERPENT256,
        /*
         *  Ron’s Cipher 2 in the 40 and 128 bit variants.
         */
        RFC2268_40,
        RFC2268_128,
        /*
         *  A 128 bit cipher as described by RFC4269.
         */
        SEED,
        /*
         *  The Camellia cipher by NTT. See http://info.isl.ntt.co.jp/crypt/eng/camellia/specifications.html.
         */
        CAMELLIA128,
        CAMELLIA192,
        CAMELLIA256,
        /*
         *  This is the Salsa20 stream cipher.
         */
        SALSA20,
        /*
         *  This is the Salsa20/12 - reduced round version of Salsa20 stream cipher.
         */
        SALSA20R12,
        /*
         *  The GOST 28147-89 cipher, defined in the respective GOST standard. Translation of this GOST into English is provided in the RFC-5830.
         */
        GOST28147,
        /*
         *  This is the ChaCha20 stream cipher.
         */
        CHACHA20
    };

    enum CipherMode {
        /*
         *  Electronic Codebook mode.
         */
        ECB,
        /*
         *  Cipher Feedback mode. For GCRY_CIPHER_MODE_CFB the shift size
         *  equals the block size of the cipher (e.g. for AES it is CFB-128).
         *  For GCRY_CIPHER_MODE_CFB8 the shift size is 8 bit but that variant
         *  is not yet available.
         */
        CFB,
        CFB8,
        /*
         *  Cipher Block Chaining mode.
         */
        CBC,
        /*
         *  Stream mode, only to be used with stream cipher algorithms.
         */
        STREAM,
        /*
         *  Output Feedback mode.
         */
        OFB,
        /*
         *  Counter mode.
         */
        CTR,
        /*
         *  This mode is used to implement the AES-Wrap algorithm according to
         *  RFC-3394. It may be used with any 128 bit block length algorithm,
         *  however the specs require one of the 3 AES algorithms. These
         *  special conditions apply: If gcry_cipher_setiv has not been used
         *  the standard IV is used; if it has been used the lower 64 bit of
         *  the IV are used as the Alternative Initial Value. On encryption
         *  the provided output buffer must be 64 bit (8 byte) larger than the
         *  input buffer; in-place encryption is still allowed. On decryption
         *  the output buffer may be specified 64 bit (8 byte) shorter than
         *  then input buffer. As per specs the input length must be at least
         *  128 bits and the length must be a multiple of 64 bits.
         */
        AESWRAP,
        /*
         *  Counter with CBC-MAC mode is an Authenticated Encryption with
         *  Associated Data (AEAD) block cipher mode, which is specified in
         *  ’NIST Special Publication 800-38C’ and RFC 3610.
         */
        CCM,
        /*
         *  Galois/Counter Mode (GCM) is an Authenticated Encryption with
         *  Associated Data (AEAD) block cipher mode, which is specified in
         *  ’NIST Special Publication 800-38D’.
         */
        GCM,
        /*
         *  This mode implements the Poly1305 Authenticated Encryption with
         *  Associated Data (AEAD) mode according to RFC-7539. This mode can be
         *  used with ChaCha20 stream cipher.
         */
        POLY1305,
        /*
         *  OCB is an Authenticated Encryption with Associated Data (AEAD)
         *  block cipher mode, which is specified in RFC-7253. Supported tag
         *  lengths are 128, 96, and 64 bit with the default being 128 bit. To
         *  switch to a different tag length gcry_cipher_ctl using the command
         *  GCRYCTL_SET_TAGLEN and the address of an int variable set to 12
         *  (for 96 bit) or 8 (for 64 bit) provided for the buffer argument and
         *  sizeof(int) for buflen.
         *
         *  Note that the use of gcry_cipher_final is required.
         */
        OCB,
        /*
         *  XEX-based tweaked-codebook mode with ciphertext stealing (XTS) mode
         *  is used to implement the AES-XTS as specified in IEEE 1619 Standard
         *  Architecture for Encrypted Shared Storage Media and NIST SP800-38E.
         *
         *  The XTS mode requires doubling key-length, for example,
         *  using 512-bit key with AES-256 (GCRY_CIPHER_AES256). The 128-bit
         *  tweak value is feed to XTS mode as little-endian byte array using
         *  gcry_cipher_setiv function. When encrypting or decrypting,
         *  full-sized data unit buffers needs to be passed to
         *  gcry_cipher_encrypt or gcry_cipher_decrypt. The tweak value
         *  is automatically incremented after each call of gcry_cipher_encrypt
         *  and gcry_cipher_decrypt. Auto-increment allows avoiding need of
         *  setting IV between processing of sequential data units.
         */
        //XTS
    };

    enum CipherFlags {
        /*
         *  Make sure that all operations are allocated in secure memory. This
         *  is useful when the key material is highly confidential.
         */
        SECURE,
        /*
         *  This flag enables the CFB sync mode, which is a special feature of
         *  Libgcrypt’s CFB mode implementation to allow for OpenPGP’s CFB
         *  variant. See gcry_cipher_sync.
         */
        ENABLE_SYNC,
        /*
         *  Enable cipher text stealing (CTS) for the CBC mode. Cannot be used
         *  simultaneous as GCRY_CIPHER_CBC_MAC. CTS mode makes it possible to
         *  transform data of almost arbitrary size (only limitation is that it
         *  must be greater than the algorithm’s block size).
         */
        CBC_CTS,
        /*
         *  Compute CBC-MAC keyed checksums. This is the same as CBC mode, but
         *  only output the last block. Cannot be used simultaneous as
         *  GCRY_CIPHER_CBC_CTS.
         */
        CBC_MAC
    };

    virtual ~QCipher();
    QCipher(CipherAlgorithm algo,
            CipherMode mode,
            CipherFlags flags,
            const QSecureMemory key,
            const QSecureMemory iv,
            const QSecureMemory ctr=QSecureMemory());

    bool reset(const QSecureMemory iv,
               const QSecureMemory ctr=QSecureMemory());
    bool add_auth_data(const QSecureMemory aad);
    bool auth_tag(QSecureMemory tag);
    bool auth_tag_check(const QSecureMemory tag);
    bool encrypt(QSecureMemory out,
                 const QSecureMemory in=QSecureMemory(),
                 bool final=false);
    bool decrypt(QSecureMemory out,
                 const QSecureMemory in=QSecureMemory(),
                 bool final=false);

private:
#ifndef USE_WIN_CRYPTO_API
    gcry_cipher_hd_t _hd;
#else
#   error   not implemented !
#endif
};

#endif // QCIPHER_H
