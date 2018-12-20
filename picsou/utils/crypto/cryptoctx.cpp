/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "cryptoctx.h"
#include "utils/macro.h"
#include "third-party/build/include/gcrypt.h"

/*
 * Warning:
 *      Picsou attempts to reduce presence of secrets in memory (and disk through swap/hibernation) yet
 *      it does not guarantees that secrets cannot be retrieved through memory dumping, hibernation file carving, etc.
 */

static const int CRYPTO_IV_SIZE=16;
static const int CRYPTO_BLK_SIZE=16;
static const int CRYPTO_TAG_SIZE=16;
static const int CRYPTO_KEY_SIZE=16;
static const int CRYPTO_SALT_SIZE=32;
static const unsigned long CRYPTO_ITER_COUNT=1000;

#define CHECK_LIB_INITIALIZATION() \
    do { \
        if(!lib_initialized()) { \
            LOG_CRITICAL("underlying cryptographic library needs to be initialized first."); \
            LOG_BOOL_RETURN(false); \
        } \
    } while(0)

#define CHECK_SALT_CACHED() \
    do { \
        if(!salt_cached()) { \
            LOG_CRITICAL("operation requires salt to be cached."); \
            LOG_BOOL_RETURN(false); \
        } \
    } while(0)

#define CHECK_DPK_CACHED() \
    do { \
        if(!dpk_cached()) { \
            LOG_CRITICAL("operation requires dpk to be cached."); \
            LOG_BOOL_RETURN(false); \
        } \
    } while(0)

#define LOG_CRITICAL_GCRYPT(message, err) \
    LOG_CRITICAL(message); \
    LOG_CRITICAL("libgcrypt error: "<<gcry_strerror(err))

bool CryptoCtx::lib_init()
{
    LOG_IN_VOID();
    /* Version check should be the very first call because it
       makes sure that important subsystems are initialized. */
    if(!gcry_check_version(GCRYPT_VERSION)) {
        LOG_CRITICAL("libgcrypt version mismatch.");
        LOG_BOOL_RETURN(false);
    }
    if(lib_initialized()) {
        LOG_CRITICAL("Crypto::lib_init() has been called already.");
        LOG_BOOL_RETURN(false);
    }
    /*
     * We don't want to see any warnings, e.g. because we have not yet
     * parsed program options which might be used to suppress such
     * warnings.
     */
    gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);
    /*
     * Allocate a pool of 16k secure memory.  This makes the secure memory
     * available and also drops privileges where needed.  Note that by
     * using functions like gcry_xmalloc_secure and gcry_mpi_snew Libgcrypt
     * may expand the secure memory pool with memory which lacks the
     * property of not being swapped out to disk.
     */
    gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);
    /*
     * It is now okay to let Libgcrypt complain when there was/is
     * a problem with the secure memory.
     */
    gcry_control(GCRYCTL_RESUME_SECMEM_WARN);
    /* Tell Libgcrypt that initialization has completed. */
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
    LOG_BOOL_RETURN(true);
}

QString CryptoCtx::lib_name()
{
    return "libgcrypt";
}

QString CryptoCtx::lib_version()
{
    return GCRYPT_VERSION;
}

CryptoCtx::CryptoCtx(const QString &wkey,
                     const QString &wsalt) :
    m_wkey(wkey),
    m_dpk(nullptr),
    m_salt()
{
    if(!wsalt.isNull()) {
        m_salt=QByteArray::fromBase64(wsalt.toUtf8());
    }
}

bool CryptoCtx::init(const QString &pswd)
{
    LOG_IN("pswd");
    CHECK_LIB_INITIALIZATION();
    /* if salt is already cached something is wrong */
    if(salt_cached()) {
        LOG_CRITICAL("crypto context has been initialized already.");
        LOG_BOOL_RETURN(false);
    }
    /* generate new salt */
    QByteArray salt=CryptoBuffer::random_ba(CRYPTO_SALT_SIZE, CryptoBuffer::RANDOM_STRONG);
    CryptoBufferShPtr mk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!derive(pswd, salt, mk)) {
        LOG_CRITICAL("key derivation failed.");
        LOG_BOOL_RETURN(false);
    }
    /* generate new DPK */
    CryptoBufferShPtr dpk=CryptoBuffer::random(CRYPTO_KEY_SIZE, CryptoBuffer::RANDOM_VERY_STRONG);
    if(!encrypt(mk, dpk, m_wkey)) {
        LOG_CRITICAL("failed to wrap DPK.");
        LOG_BOOL_RETURN(false);
    }
    /* success */
    m_salt=salt;
    m_dpk=dpk;
    LOG_BOOL_RETURN(true);
}

bool CryptoCtx::wrap(const QByteArray &cdata, QString &wdata) const
{
    LOG_IN("cdata,wdata");
    CHECK_LIB_INITIALIZATION();
    CHECK_SALT_CACHED();
    CHECK_DPK_CACHED();
    /* encrypt given data */
    if(!encrypt(m_dpk, cdata, wdata)) {
        LOG_CRITICAL("data encryption failed.");
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

bool CryptoCtx::unwrap(const QString &pswd, const QString &wdata, QByteArray &cdata)
{
    LOG_IN("pswd,wdata,cdata");
    CHECK_LIB_INITIALIZATION();
    CHECK_SALT_CACHED();
    /* if DPK is already cached something is wrong */
    if(dpk_cached()) {
        LOG_CRITICAL("dpk is already cached meaning the object has been unwrapped already.");
        LOG_BOOL_RETURN(false);
    }
    /* retrieve MK */
    CryptoBufferShPtr mk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!derive(pswd, m_salt, mk)) {
        LOG_CRITICAL("key derivation failed.");
        LOG_BOOL_RETURN(false);
    }
    /* decrypt DPK with MK */
    CryptoBufferShPtr dpk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!decrypt(mk, m_wkey, dpk)) {
        LOG_CRITICAL("invalid user password.");
        LOG_BOOL_RETURN(false);
    }
    /* decrypt data with DPK */
    if(!decrypt(dpk, wdata, cdata)) {
        LOG_CRITICAL("failed to decrypt data.");
        LOG_BOOL_RETURN(false);
    }
    m_dpk=dpk;
    LOG_BOOL_RETURN(true);
}

bool CryptoCtx::rewrap(const QString &prev_pswd, const QString &next_pswd)
{
    LOG_IN("prev_pswd,next_pswd");
    CHECK_LIB_INITIALIZATION();
    CHECK_SALT_CACHED();
    /* compute current MK */
    CryptoBufferShPtr prev_mk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!derive(prev_pswd, m_salt, prev_mk)) {
        LOG_CRITICAL("prev key derivation failed.");
        LOG_BOOL_RETURN(false);
    }
    /* decrypt DPK using current MK */
    CryptoBufferShPtr dpk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!decrypt(prev_mk, m_wkey, dpk)) {
        LOG_CRITICAL("invalid user password.");
        LOG_BOOL_RETURN(false);
    }
    /* compute next MK */
    CryptoBufferShPtr next_mk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!derive(next_pswd, m_salt, next_mk)) {
        LOG_CRITICAL("next key derivation failed.");
        LOG_BOOL_RETURN(false);
    }
    /* encrypt DPK with next MK */
    if(!encrypt(next_mk, dpk, m_wkey)) {
        LOG_CRITICAL("failed to wrap DPK.");
        LOG_BOOL_RETURN(false);
    }
    LOG_INFO("rewraping succeeded.");
    LOG_BOOL_RETURN(true);
}

bool CryptoCtx::lib_initialized()
{
    return gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P)!=GPG_ERR_NO_ERROR;
}

bool CryptoCtx::derive(const QString &pswd, const QByteArray &salt, CryptoBufferShPtr &key) const
{
    LOG_IN("pswd,key");
    CHECK_LIB_INITIALIZATION();
    QByteArray pswdba=pswd.toUtf8();
    gcry_error_t err;
    if((err=gcry_kdf_derive(pswdba.data(), static_cast<size_t>(pswdba.size()),
                             GCRY_KDF_PBKDF2, GCRY_MD_SHA1,
                             salt.data(), static_cast<size_t>(salt.size()),
                             CRYPTO_ITER_COUNT,
                             static_cast<size_t>(key->size()), key->rwbuf()))) {
        LOG_CRITICAL_GCRYPT("failed to derive key from given password.", err);
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

bool cipher_prepare(gcry_cipher_hd_t *hd, const CryptoBufferShPtr &key, const QByteArray &iv)
{
    LOG_IN("hd="<<hd<<",key,iv");
    gcry_error_t err;
    /* open a new cipher */
    if((err=gcry_cipher_open(hd, GCRY_CIPHER_AES, GCRY_CIPHER_MODE_GCM, GCRY_CIPHER_SECURE))) {
        LOG_CRITICAL_GCRYPT("failed to create encryption cipher.", err);
        LOG_BOOL_RETURN(false);
    }
    /* set key */
    if((err=gcry_cipher_setkey(*hd, key->rbuf(), static_cast<size_t>(key->size())))) {
        LOG_CRITICAL_GCRYPT("failed to set cipher key.", err);
        LOG_BOOL_RETURN(false);
    }
    /* set IV */
    if((err=gcry_cipher_setiv(*hd, iv.data(), static_cast<size_t>(iv.size())))) {
        LOG_CRITICAL_GCRYPT("failed to set cipher IV.", err);
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

QByteArray cipher_gettag(gcry_cipher_hd_t *hd)
{
    QByteArray tag(CRYPTO_TAG_SIZE, 0);
    gcry_error_t err;
    /* retrieve tag from cipher */
    if((err=gcry_cipher_gettag(*hd, tag.data(), static_cast<size_t>(tag.size())))) {
        LOG_CRITICAL_GCRYPT("failed to get cipher tag.", err);
        return QByteArray();
    }
    return tag;
}

bool cipher_checktag(gcry_cipher_hd_t *hd, const QByteArray &tag)
{
    gcry_error_t err;
    /* check previous tag with current cipher tag */
    if((err=gcry_cipher_checktag(*hd, tag.data(), static_cast<size_t>(tag.size())))) {
        LOG_CRITICAL_GCRYPT("failed to get cipher tag.", err);
        return false;
    }
    return true;
}

bool CryptoCtx::encrypt(const CryptoBufferShPtr &key, const QByteArray &in, QString &out) const
{
    LOG_IN("key,cdata,b64data");
    gcry_cipher_hd_t hd;
    /* prepare cipher */
    QByteArray iv=CryptoBuffer::random_ba(CRYPTO_IV_SIZE, CryptoBuffer::RANDOM_STRONG);
    if(!cipher_prepare(&hd, key, iv)) {
        LOG_CRITICAL("failed to prepare encryption cipher.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* compute and apply padding */
    int pad=in.size()%CRYPTO_BLK_SIZE;
    if(pad==0) {
        pad=CRYPTO_BLK_SIZE;
    }
    /* perform padding */
    QByteArray encbuf=in;
    encbuf.append(pad, static_cast<char>(pad));
    size_t padded_cdata_sz=static_cast<size_t>(encbuf.size());
    /* encrypt buffer */
    gcry_error_t err;
    if((err=gcry_cipher_encrypt(hd, encbuf.data(), padded_cdata_sz, nullptr, 0))) {
        LOG_CRITICAL_GCRYPT("failed to encrypt data.", err);
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* finalize cipher */
    QByteArray tag=cipher_gettag(&hd);
    if(tag.isEmpty()) {
        LOG_CRITICAL("failed to get cipher tag.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* pack output buffer */
    out=QByteArray(iv+encbuf+tag).toBase64();
    /* success */
    gcry_cipher_close(hd);
    LOG_BOOL_RETURN(true);
}

bool CryptoCtx::encrypt(const CryptoBufferShPtr &key, const CryptoBufferShPtr &in, QString &out) const
{
    LOG_IN("key,cdata,b64data");
    gcry_cipher_hd_t hd;
    /* prepare cipher */
    QByteArray iv=CryptoBuffer::random_ba(CRYPTO_IV_SIZE, CryptoBuffer::RANDOM_STRONG);
    if(!cipher_prepare(&hd, key, iv)) {
        LOG_CRITICAL("failed to prepare encryption cipher.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* perform padding */
    int pad=in->size()%CRYPTO_BLK_SIZE;
    if(pad==0) {
        pad=CRYPTO_BLK_SIZE;
    }
    CryptoBufferShPtr encbuf=CryptoBuffer::malloc(in->size()+pad);
    memset(encbuf->rwbuf(), pad, static_cast<size_t>(encbuf->size()));
    memcpy(encbuf->rwbuf(), in->rbuf(), static_cast<size_t>(in->size()));
    /* encrypt buffer */
    gcry_error_t err;
    if((err=gcry_cipher_encrypt(hd, encbuf->rwbuf(), static_cast<size_t>(encbuf->size()), nullptr, 0))) {
        LOG_CRITICAL_GCRYPT("failed to encrypt data.", err);
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* finalize cipher */
    QByteArray tag=cipher_gettag(&hd);
    if(tag.isEmpty()) {
        LOG_CRITICAL("failed to get cipher tag.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* pack output buffer */
    QByteArray edata(iv);
    edata.append(encbuf->rbuf(), encbuf->size());
    edata.append(tag);
    out=edata.toBase64();
    /* success */
    gcry_cipher_close(hd);
    LOG_BOOL_RETURN(true);
}

bool CryptoCtx::decrypt(const CryptoBufferShPtr &key, const QString &in, QByteArray &out) const
{
    LOG_IN("key,b64data,cdata");
    gcry_cipher_hd_t hd;
    /* decode edata */
    QByteArray encbuf=QByteArray::fromBase64(in.toUtf8());
    QByteArray iv(encbuf.data(), CRYPTO_IV_SIZE);
    QByteArray tag(encbuf.data()+(encbuf.size()-CRYPTO_TAG_SIZE), CRYPTO_TAG_SIZE);
    QByteArray decbuf=encbuf.mid(CRYPTO_IV_SIZE, encbuf.size()-CRYPTO_IV_SIZE-CRYPTO_TAG_SIZE);
    /* prepare cipher */
    if(!cipher_prepare(&hd, key, iv)) {
        LOG_CRITICAL("failed to prepare encryption cipher.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* decrypt buffer */
    gcry_error_t err;
    if((err=gcry_cipher_decrypt(hd, decbuf.data(), static_cast<size_t>(decbuf.size()), nullptr, 0))) {
        LOG_CRITICAL_GCRYPT("failed to get cipher tag.", err);
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* remove padding */
    decbuf.chop(decbuf.back());
    /* finalize cipher */
    if(!cipher_checktag(&hd, tag)) {
        LOG_CRITICAL("cipher tag check failed.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* success */
    out=decbuf;
    gcry_cipher_close(hd);
    LOG_BOOL_RETURN(true);
}

bool CryptoCtx::decrypt(const CryptoBufferShPtr &key, const QString &in, CryptoBufferShPtr &out) const
{
    LOG_IN("key,b64data,cdata");
    gcry_cipher_hd_t hd;
    /* decode edata */
    QByteArray encbuf=QByteArray::fromBase64(in.toUtf8());
    QByteArray iv(encbuf.data(), CRYPTO_IV_SIZE);
    QByteArray tag(encbuf.data()+(encbuf.size()-CRYPTO_TAG_SIZE), CRYPTO_TAG_SIZE);
    encbuf=encbuf.mid(CRYPTO_IV_SIZE, encbuf.size()-CRYPTO_IV_SIZE-CRYPTO_TAG_SIZE);
    /* prepare cipher */
    if(!cipher_prepare(&hd, key, iv)) {
        LOG_CRITICAL("failed to prepare encryption cipher.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* decrypt buffer */
    gcry_error_t err;
    CryptoBufferShPtr decbuf=CryptoBuffer::malloc(encbuf.size());
    memcpy(decbuf->rwbuf(), encbuf, static_cast<size_t>(decbuf->size()));
    if((err=gcry_cipher_decrypt(hd, decbuf->rwbuf(), static_cast<size_t>(decbuf->size()), nullptr, 0))) {
        LOG_CRITICAL_GCRYPT("failed to get cipher tag.", err);
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* remove padding */
    int pad=decbuf->rbuf()[decbuf->size()-1];
    int unpadded_sz=decbuf->size()-pad;
    if(unpadded_sz<=0) {
        LOG_CRITICAL("invalid unpadded size.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    CryptoBufferShPtr unpadded=CryptoBuffer::malloc(unpadded_sz);
    memcpy(unpadded->rwbuf(), decbuf->rbuf(), static_cast<size_t>(unpadded_sz));
    /* finalize cipher */
    if(!cipher_checktag(&hd, tag)) {
        LOG_CRITICAL("cipher tag check failed.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* success */
    out=unpadded;
    gcry_cipher_close(hd);
    LOG_BOOL_RETURN(true);
}
