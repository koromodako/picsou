#include "qcipher.h"
#include "utils/macro.h"
#include <gcrypt.h>

QCipher::~QCipher()
{
    if(valid()) {
        gcry_cipher_close(_hd);
    }
}

QCipher::QCipher(CipherAlgorithm algo,
                 CipherMode mode,
                 CipherFlags flags,
                 const QSecureMemory key,
                 const QSecureMemory iv,
                 const QSecureMemory ctr) :
    QCryptoWrapper()
{
    int calgo;
    switch (algo) {
        case IDEA: calgo=GCRY_CIPHER_IDEA; break;
        case _3DES: calgo=GCRY_CIPHER_3DES; break;
        case CAST5: calgo=GCRY_CIPHER_CAST5; break;
        case BLOWFISH: calgo=GCRY_CIPHER_BLOWFISH; break;
        case SAFER_SK128: calgo=GCRY_CIPHER_SAFER_SK128; break;
        case DES_SK: calgo=GCRY_CIPHER_DES_SK; break;
        case AES: calgo=GCRY_CIPHER_AES; break;
        case AES192: calgo=GCRY_CIPHER_AES192; break;
        case AES256: calgo=GCRY_CIPHER_AES256; break;
        case TWOFISH: calgo=GCRY_CIPHER_TWOFISH; break;
        case TWOFISH128: calgo=GCRY_CIPHER_TWOFISH128; break;
        case ARCFOUR: calgo=GCRY_CIPHER_ARCFOUR; break;
        case DES: calgo=GCRY_CIPHER_DES; break;
        case SERPENT128: calgo=GCRY_CIPHER_SERPENT128; break;
        case SERPENT192: calgo=GCRY_CIPHER_SERPENT192; break;
        case SERPENT256: calgo=GCRY_CIPHER_SERPENT256; break;
        case RFC2268_40: calgo=GCRY_CIPHER_RFC2268_40; break;
        case RFC2268_128: calgo=GCRY_CIPHER_RFC2268_128; break;
        case SEED: calgo=GCRY_CIPHER_SEED; break;
        case CAMELLIA128: calgo=GCRY_CIPHER_CAMELLIA128; break;
        case CAMELLIA192: calgo=GCRY_CIPHER_CAMELLIA192; break;
        case CAMELLIA256: calgo=GCRY_CIPHER_CAMELLIA256; break;
        case SALSA20: calgo=GCRY_CIPHER_SALSA20; break;
        case SALSA20R12: calgo=GCRY_CIPHER_SALSA20R12; break;
        case GOST28147: calgo=GCRY_CIPHER_GOST28147; break;
        case CHACHA20: calgo=GCRY_CIPHER_CHACHA20; break;
    }
    int cmode;
    switch (mode) {
        case ECB: cmode=GCRY_CIPHER_MODE_ECB; break;
        case CFB: cmode=GCRY_CIPHER_MODE_CFB; break;
        case CFB8: cmode=GCRY_CIPHER_MODE_CFB8; break;
        case CBC: cmode=GCRY_CIPHER_MODE_CBC; break;
        case STREAM: cmode=GCRY_CIPHER_MODE_STREAM; break;
        case OFB: cmode=GCRY_CIPHER_MODE_OFB; break;
        case CTR: cmode=GCRY_CIPHER_MODE_CTR; break;
        case AESWRAP: cmode=GCRY_CIPHER_MODE_AESWRAP; break;
        case CCM: cmode=GCRY_CIPHER_MODE_CCM; break;
        case GCM: cmode=GCRY_CIPHER_MODE_GCM; break;
        case POLY1305: cmode=GCRY_CIPHER_MODE_POLY1305; break;
        case OCB: cmode=GCRY_CIPHER_MODE_OCB; break;
        //case XTS: cmode=GCRY_CIPHER_MODE_XTS; break;
    }
    uint cipher_flags=0;
    cipher_flags|=(IS_FLAG_SET(flags, SECURE)? GCRY_CIPHER_SECURE: 0);
    cipher_flags|=(IS_FLAG_SET(flags, ENABLE_SYNC)? GCRY_CIPHER_ENABLE_SYNC: 0);
    cipher_flags|=(IS_FLAG_SET(flags, CBC_CTS)? GCRY_CIPHER_CBC_CTS: 0);
    cipher_flags|=(IS_FLAG_SET(flags, CBC_MAC)? GCRY_CIPHER_CBC_MAC: 0);
    if(!wrap(gcry_cipher_open(&_hd, calgo, cmode, cipher_flags))) {
        LOG_VOID_RETURN();
    }
    if(!wrap(gcry_cipher_setkey(_hd, key.const_data(), key.length()))) {
        gcry_cipher_close(_hd); _hd=nullptr;
        LOG_VOID_RETURN();
    }
    if(!wrap(gcry_cipher_setiv(_hd, iv.const_data(), iv.length()))) {
        gcry_cipher_close(_hd); _hd=nullptr;
        LOG_VOID_RETURN();
    }
    if(wrap(gcry_cipher_setctr(_hd, ctr.const_data(), ctr.length()))) {
        gcry_cipher_close(_hd); _hd=nullptr;
        LOG_VOID_RETURN();
    }
    LOG_VOID_RETURN();
}

bool QCipher::valid() const
{
    return (_hd!=nullptr);
}

bool QCipher::reset(const QSecureMemory iv,
                    const QSecureMemory ctr)
{
    LOG_IN("iv=*****,ctr=*****");
    if(!wrap(gcry_cipher_reset(_hd))) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_cipher_setiv(_hd, iv.const_data(), iv.length()))) {
        LOG_BOOL_RETURN(false);
    }
    if(!wrap(gcry_cipher_setctr(_hd, ctr.const_data(), ctr.length()))) {
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
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
    LOG_IN("out,in,final");
    if(final) {
        if(!wrap(gcry_cipher_final(_hd))) {
            LOG_BOOL_RETURN(false);
        }
    }
    LOG_BOOL_RETURN(wrap(gcry_cipher_encrypt(_hd,
                                             out.data(), out.length(),
                                             in.const_data(), in.length())));
}

bool QCipher::decrypt(QSecureMemory out,
                      const QSecureMemory in,
                      bool final)
{
    LOG_IN("out,in,final");
    if(final) {
        if(!wrap(gcry_cipher_final(_hd))) {
            LOG_BOOL_RETURN(false);
        }
    }

    LOG_BOOL_RETURN(wrap(gcry_cipher_decrypt(_hd,
                                             out.data(), out.length(),
                                             in.const_data(), in.length())));
}
