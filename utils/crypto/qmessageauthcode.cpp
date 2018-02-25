#include "qmessageauthcode.h"

#ifndef USE_WIN_CRYPTO_API
#   include <gcrypt.h>
#else
#   error   not implemented !
#endif

#include "utils/macro.h"

QMessageAuthCode::~QMessageAuthCode()
{
    if(valid()) {
        gcry_mac_close(_hd);
    }
}

QMessageAuthCode::QMessageAuthCode(MACAlgorithm algo,
                                   MACFlag flags,
                                   const QSecureMemory key,
                                   const QSecureMemory iv) :
    QCryptoWrapper()
{
    int mac_algo;
    uint mac_flags;

    switch (algo) {
        case HMAC_SHA256: mac_algo=GCRY_MAC_HMAC_SHA256; break;
        case HMAC_SHA224: mac_algo=GCRY_MAC_HMAC_SHA224; break;
        case HMAC_SHA512: mac_algo=GCRY_MAC_HMAC_SHA512; break;
        case HMAC_SHA384: mac_algo=GCRY_MAC_HMAC_SHA384; break;
        case HMAC_SHA3_256: mac_algo=GCRY_MAC_HMAC_SHA3_256; break;
        case HMAC_SHA3_224: mac_algo=GCRY_MAC_HMAC_SHA3_224; break;
        case HMAC_SHA3_512: mac_algo=GCRY_MAC_HMAC_SHA3_512; break;
        case HMAC_SHA3_384: mac_algo=GCRY_MAC_HMAC_SHA3_384; break;
        case HMAC_SHA1: mac_algo=GCRY_MAC_HMAC_SHA1; break;
        case HMAC_MD5: mac_algo=GCRY_MAC_HMAC_MD5; break;
        case HMAC_MD4: mac_algo=GCRY_MAC_HMAC_MD4; break;
        case HMAC_RMD160: mac_algo=GCRY_MAC_HMAC_RMD160; break;
        case HMAC_WHIRLPOOL: mac_algo=GCRY_MAC_HMAC_WHIRLPOOL; break;
        case HMAC_GOSTR3411_94: mac_algo=GCRY_MAC_HMAC_GOSTR3411_94; break;
        case HMAC_STRIBOG256: mac_algo=GCRY_MAC_HMAC_STRIBOG256; break;
        case HMAC_STRIBOG512: mac_algo=GCRY_MAC_HMAC_STRIBOG512; break;
        case CMAC_AES: mac_algo=GCRY_MAC_CMAC_AES; break;
        case CMAC_3DES: mac_algo=GCRY_MAC_CMAC_3DES; break;
        case CMAC_CAMELLIA: mac_algo=GCRY_MAC_CMAC_CAMELLIA; break;
        case CMAC_CAST5: mac_algo=GCRY_MAC_CMAC_CAST5; break;
        case CMAC_BLOWFISH: mac_algo=GCRY_MAC_CMAC_BLOWFISH; break;
        case CMAC_TWOFISH: mac_algo=GCRY_MAC_CMAC_TWOFISH; break;
        case CMAC_SERPENT: mac_algo=GCRY_MAC_CMAC_SERPENT; break;
        case CMAC_SEED: mac_algo=GCRY_MAC_CMAC_SEED; break;
        case CMAC_RFC2268: mac_algo=GCRY_MAC_CMAC_RFC2268; break;
        case CMAC_IDEA: mac_algo=GCRY_MAC_CMAC_IDEA; break;
        case CMAC_GOST28147: mac_algo=GCRY_MAC_CMAC_GOST28147; break;
        case GMAC_AES: mac_algo=GCRY_MAC_GMAC_AES; break;
        case GMAC_CAMELLIA: mac_algo=GCRY_MAC_GMAC_CAMELLIA; break;
        case GMAC_TWOFISH: mac_algo=GCRY_MAC_GMAC_TWOFISH; break;
        case GMAC_SERPENT: mac_algo=GCRY_MAC_GMAC_SERPENT; break;
        case GMAC_SEED: mac_algo=GCRY_MAC_GMAC_SEED; break;
        case POLY1305: mac_algo=GCRY_MAC_POLY1305; break;
        case POLY1305_AES: mac_algo=GCRY_MAC_POLY1305_AES; break;
        case POLY1305_CAMELLIA: mac_algo=GCRY_MAC_POLY1305_CAMELLIA; break;
        case POLY1305_TWOFISH: mac_algo=GCRY_MAC_POLY1305_TWOFISH; break;
        case POLY1305_SERPENT: mac_algo=GCRY_MAC_POLY1305_SERPENT; break;
        case POLY1305_SEED: mac_algo=GCRY_MAC_POLY1305_SEED; break;
    }

    mac_flags=0;
    mac_flags|=(IS_FLAG_SET(flags, SECURE)? GCRY_MAC_FLAG_SECURE: 0);

    if(!wrap(gcry_mac_open(&_hd, mac_algo, mac_flags, nullptr))) {
        goto end;
    }

    if(!wrap(gcry_mac_setkey(_hd, key.const_data(), key.length()))){
        goto failed;
    }

    if(!wrap(gcry_mac_setiv(_hd, iv.const_data(), iv.length()))) {
        goto failed;
    }

    goto end;

failed:
    gcry_mac_close(_hd);
    _hd=nullptr;

end:
    return;
}

bool QMessageAuthCode::valid()
{
    return (_hd!=nullptr);
}

bool QMessageAuthCode::reset(const QSecureMemory iv)
{
    bool success=false;

    if(!wrap(gcry_mac_reset(_hd))) {
        goto end;
    }
    if(!wrap(gcry_mac_setiv(_hd, iv.const_data(), iv.length()))) {
        goto end;
    }

    success=true;
end:
    return success;
}

bool QMessageAuthCode::update(const QSecureMemory data)
{
    return wrap(gcry_mac_write(_hd, data.const_data(), data.length()));
}

bool QMessageAuthCode::verify(const QSecureMemory mac)
{
    return wrap(gcry_mac_verify(_hd, mac.const_data(), mac.length()));
}

bool QMessageAuthCode::digest(QSecureMemory mac)
{
    size_t buflen = mac.length();

    if(!wrap(gcry_mac_read(_hd, mac.data(), &buflen))) {
        goto failed;
    }

    if(buflen<mac.length()) {
        mac.resize(buflen);
    }

    return true;
failed:
    return false;
}
