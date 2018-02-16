#include "qcipher.h"

#ifndef USE_WIN_CRYPTO_API
#   include <gcrypt.h>
#else
#   error   not implemented !
#endif

#include "utils/macro.h"

QCipher::~QCipher()
{
    gcry_cipher_close(_hd);
    _hd=nullptr;
}

QCipher::QCipher(CipherAlgorithm algo,
                 CipherMode mode,
                 CipherFlags flags,
                 const QSecureMemory key,
                 const QSecureMemory iv,
                 const QSecureMemory ctr) :
    QCryptoWrapper()
{
    int cipher_algo;
    int cipher_mode;
    uint cipher_flags;

    switch (algo) {
        case IDEA: cipher_algo=GCRY_CIPHER_IDEA; break;
        case _3DES: cipher_algo=GCRY_CIPHER_3DES; break;
        case CAST5: cipher_algo=GCRY_CIPHER_CAST5; break;
        case BLOWFISH: cipher_algo=GCRY_CIPHER_BLOWFISH; break;
        case SAFER_SK128: cipher_algo=GCRY_CIPHER_SAFER_SK128; break;
        case DES_SK: cipher_algo=GCRY_CIPHER_DES_SK; break;
        case AES: cipher_algo=GCRY_CIPHER_AES; break;
        case AES192: cipher_algo=GCRY_CIPHER_AES192; break;
        case AES256: cipher_algo=GCRY_CIPHER_AES256; break;
        case TWOFISH: cipher_algo=GCRY_CIPHER_TWOFISH; break;
        case TWOFISH128: cipher_algo=GCRY_CIPHER_TWOFISH128; break;
        case ARCFOUR: cipher_algo=GCRY_CIPHER_ARCFOUR; break;
        case DES: cipher_algo=GCRY_CIPHER_DES; break;
        case SERPENT128: cipher_algo=GCRY_CIPHER_SERPENT128; break;
        case SERPENT192: cipher_algo=GCRY_CIPHER_SERPENT192; break;
        case SERPENT256: cipher_algo=GCRY_CIPHER_SERPENT256; break;
        case RFC2268_40: cipher_algo=GCRY_CIPHER_RFC2268_40; break;
        case RFC2268_128: cipher_algo=GCRY_CIPHER_RFC2268_128; break;
        case SEED: cipher_algo=GCRY_CIPHER_SEED; break;
        case CAMELLIA128: cipher_algo=GCRY_CIPHER_CAMELLIA128; break;
        case CAMELLIA192: cipher_algo=GCRY_CIPHER_CAMELLIA192; break;
        case CAMELLIA256: cipher_algo=GCRY_CIPHER_CAMELLIA256; break;
        case SALSA20: cipher_algo=GCRY_CIPHER_SALSA20; break;
        case SALSA20R12: cipher_algo=GCRY_CIPHER_SALSA20R12; break;
        case GOST28147: cipher_algo=GCRY_CIPHER_GOST28147; break;
        case CHACHA20: cipher_algo=GCRY_CIPHER_CHACHA20; break;
    }

    switch (mode) {
        case ECB: cipher_mode=GCRY_CIPHER_MODE_ECB; break;
        case CFB: cipher_mode=GCRY_CIPHER_MODE_CFB; break;
        case CFB8: cipher_mode=GCRY_CIPHER_MODE_CFB8; break;
        case CBC: cipher_mode=GCRY_CIPHER_MODE_CBC; break;
        case STREAM: cipher_mode=GCRY_CIPHER_MODE_STREAM; break;
        case OFB: cipher_mode=GCRY_CIPHER_MODE_OFB; break;
        case CTR: cipher_mode=GCRY_CIPHER_MODE_CTR; break;
        case AESWRAP: cipher_mode=GCRY_CIPHER_MODE_AESWRAP; break;
        case CCM: cipher_mode=GCRY_CIPHER_MODE_CCM; break;
        case GCM: cipher_mode=GCRY_CIPHER_MODE_GCM; break;
        case POLY1305: cipher_mode=GCRY_CIPHER_MODE_POLY1305; break;
        case OCB: cipher_mode=GCRY_CIPHER_MODE_OCB; break;
        //case XTS: cipher_mode=GCRY_CIPHER_MODE_XTS; break;
    }

    cipher_flags = 0;
    cipher_flags |= (IS_FLAG_SET(flags, SECURE)? GCRY_CIPHER_SECURE: 0);
    cipher_flags |= (IS_FLAG_SET(flags, ENABLE_SYNC)? GCRY_CIPHER_ENABLE_SYNC: 0);
    cipher_flags |= (IS_FLAG_SET(flags, CBC_CTS)? GCRY_CIPHER_CBC_CTS: 0);
    cipher_flags |= (IS_FLAG_SET(flags, CBC_MAC)? GCRY_CIPHER_CBC_MAC: 0);

    if(!wrap(gcry_cipher_open(&_hd, cipher_algo, cipher_mode, cipher_flags))) {
        goto end;
    }

    if(!wrap(gcry_cipher_setkey(_hd, key.const_data(), key.length()))) {
        goto failed;
    }

    if(!wrap(gcry_cipher_setiv(_hd, iv.const_data(), iv.length()))) {
        goto failed;
    }

    if(wrap(gcry_cipher_setctr(_hd, ctr.const_data(), ctr.length()))) {
        goto failed;
    }

    goto end;

failed:
    gcry_cipher_close(_hd);
    _hd=nullptr;
end:
    return;
}

bool QCipher::reset(const QSecureMemory iv,
                    const QSecureMemory ctr)
{
    bool success=false;

    if(!wrap(gcry_cipher_reset(_hd))) {
        goto end;
    }

    if(!wrap(gcry_cipher_setiv(_hd, iv.const_data(), iv.length()))) {
        goto end;
    }

    if(!wrap(gcry_cipher_setctr(_hd, ctr.const_data(), ctr.length()))) {
        goto end;
    }
    success=true;
end:
    return success;
}

bool QCipher::add_auth_data(const QSecureMemory aad)
{
    return wrap(gcry_cipher_authenticate(_hd, aad.const_data(), aad.length()));
}

bool QCipher::auth_tag(QSecureMemory tag)
{
    return wrap(gcry_cipher_gettag(_hd, tag.data(), tag.length()));
}

bool QCipher::auth_tag_check(const QSecureMemory tag)
{
    return wrap(gcry_cipher_checktag(_hd, tag.const_data(), tag.length()));
}

bool QCipher::encrypt(QSecureMemory out,
                      const QSecureMemory in,
                      bool final)
{
    bool success=false;

    if(final) {
        if(!wrap(gcry_cipher_final(_hd))) {
            goto end;
        }
    }

    success=wrap(gcry_cipher_encrypt(_hd,
                                     out.data(), out.length(),
                                     in.const_data(), in.length()));
end:
    return success;
}

bool QCipher::decrypt(QSecureMemory out,
                      const QSecureMemory in,
                      bool final)
{
    bool success=false;

    if(final) {
        if(!wrap(gcry_cipher_final(_hd))) {
            goto end;
        }
    }

    success=wrap(gcry_cipher_decrypt(_hd,
                                     out.data(), out.length(),
                                     in.const_data(), in.length()));
end:
    return success;
}

