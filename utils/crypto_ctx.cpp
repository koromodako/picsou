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
#include "crypto_ctx.h"
#include "utils/macro.h"
#include <gcrypt.h>

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

CryptoBuffer::~CryptoBuffer()
{
    if(m_buf!=nullptr) {
        gcry_free(m_buf);
    }
}

CryptoBuffer::CryptoBuffer(char *buf, int size) :
    m_buf(buf),
    m_size(size)
{

}

CryptoBufferShPtr CryptoBuffer::malloc(int size)
{
    char *buf=static_cast<char*>(gcry_malloc_secure(static_cast<size_t>(size)));
    if(buf==nullptr) {
        LOG_CRITICAL("failed to allocate secure memory.");
        return CryptoBufferShPtr();
    }
    return CryptoBufferShPtr(new CryptoBuffer(buf, size));
}

CryptoBufferShPtr CryptoBuffer::calloc(int mem_size, int mem_cnt)
{
    char *buf=static_cast<char*>(gcry_calloc_secure(static_cast<size_t>(mem_cnt),
                                                    static_cast<size_t>(mem_size)));
    if(buf==nullptr) {
        LOG_CRITICAL("failed to allocate secure memory.");
        return CryptoBufferShPtr();
    }
    return CryptoBufferShPtr(new CryptoBuffer(buf, mem_cnt*mem_size));
}

CryptoBufferShPtr CryptoBuffer::random(int size, RandomLevel lvl)
{
    gcry_random_level_t level;
    switch (lvl) {
    case RANDOM_WEAK:
        level=GCRY_WEAK_RANDOM;
        break;
    case RANDOM_STRONG:
        level=GCRY_STRONG_RANDOM;
        break;
    case RANDOM_VERY_STRONG:
        level=GCRY_VERY_STRONG_RANDOM;
        break;
    }
    char *buf=static_cast<char*>(gcry_random_bytes_secure(static_cast<size_t>(size), level));
    if(buf==nullptr) {
        LOG_CRITICAL("failed to allocate secure memory.");
        return CryptoBufferShPtr();
    }
    return CryptoBufferShPtr(new CryptoBuffer(buf, size));
}

QByteArray CryptoBuffer::random_ba(int size, CryptoBuffer::RandomLevel lvl)
{
    CryptoBufferShPtr rand=random(size, lvl);
    return QByteArray(rand->rbuf(), size);
}

#define CHECK_LIB_INITIALIZATION() \
    do { \
        if(!lib_initialized()) { \
            LOG_CRITICAL("CryptoCtx::lib_init() needs to be called first."); \
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
    if(dpk_cached()) {
        LOG_CRITICAL("dpk is already cached meaning the object has been unwrapped already.");
        LOG_BOOL_RETURN(false);
    }
    CryptoBufferShPtr mk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!derive(pswd, m_salt, mk)) {
        LOG_CRITICAL("key derivation failed.");
        LOG_BOOL_RETURN(false);
    }
    m_dpk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!decrypt(mk, m_wkey, m_dpk)) {
        LOG_CRITICAL("invalid user password.");
        LOG_BOOL_RETURN(false);
    }
    if(!decrypt(m_dpk, wdata, cdata)) {
        LOG_CRITICAL("failed to decrypt data.");
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

bool CryptoCtx::rewrap(const QString &prev_pswd, const QString &next_pswd)
{
    LOG_IN("prev_pswd,next_pswd");
    CHECK_LIB_INITIALIZATION();
    CHECK_SALT_CACHED();
    CryptoBufferShPtr prev_mk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!derive(prev_pswd, m_salt, prev_mk)) {
        LOG_CRITICAL("prev key derivation failed.");
        LOG_BOOL_RETURN(false);
    }
    CryptoBufferShPtr dpk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!decrypt(prev_mk, m_wkey, dpk)) {
        LOG_CRITICAL("invalid user password.");
        LOG_BOOL_RETURN(false);
    }
    CryptoBufferShPtr next_mk=CryptoBuffer::malloc(CRYPTO_KEY_SIZE);
    if(!derive(next_pswd, m_salt, next_mk)) {
        LOG_CRITICAL("next key derivation failed.");
        LOG_BOOL_RETURN(false);
    }
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
    if(!gcry_kdf_derive(pswdba.data(), static_cast<size_t>(pswdba.size()),
                        GCRY_KDF_PBKDF2, GCRY_MD_SHA1,
                        salt.data(), static_cast<size_t>(salt.size()),
                        CRYPTO_ITER_COUNT,
                        static_cast<size_t>(key->size()), key->rwbuf())) {
        LOG_CRITICAL("failed to derive key from given password.");
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

bool cipher_prepare(gcry_cipher_hd_t *hd, const CryptoBufferShPtr &key, const QByteArray &iv)
{
    LOG_IN("hd="<<hd<<",key,iv");
    if(!gcry_cipher_open(hd, GCRY_CIPHER_AES, GCRY_CIPHER_MODE_GCM, GCRY_CIPHER_SECURE)) {
        LOG_CRITICAL("failed to create encryption cipher.");
        LOG_BOOL_RETURN(false);
    }
    /* set key */
    if(!gcry_cipher_setkey(*hd, key->rbuf(), static_cast<size_t>(key->size()))) {
        LOG_CRITICAL("failed to set cipher key.");
        LOG_BOOL_RETURN(false);
    }
    /* set IV */
    if(!gcry_cipher_setiv(*hd, iv.data(), static_cast<size_t>(iv.size()))) {
        LOG_CRITICAL("failed to set cipher IV.");
        LOG_BOOL_RETURN(false);
    }
    LOG_BOOL_RETURN(true);
}

QByteArray cipher_gettag(gcry_cipher_hd_t *hd) {
    QByteArray tag(CRYPTO_TAG_SIZE, 0);
    if(!gcry_cipher_gettag(*hd, tag.data(), static_cast<size_t>(tag.size()))) {
        LOG_CRITICAL("failed to get cipher tag.");
        return QByteArray();
    }
    return tag;
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
    QByteArray encbuf=in;
    encbuf.append(pad, static_cast<char>(pad));
    size_t padded_cdata_sz=static_cast<size_t>(encbuf.size());
    /* encrypt buffer */
    if(!gcry_cipher_encrypt(hd, encbuf.data(), padded_cdata_sz, nullptr, 0)) {
        LOG_CRITICAL("failed to encrypt data.");
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
    /* compute and apply padding */
    int pad=in->size()%CRYPTO_BLK_SIZE;
    if(pad==0) {
        pad=CRYPTO_BLK_SIZE;
    }
    CryptoBufferShPtr encbuf=CryptoBuffer::malloc(in->size()+pad);
    memset(encbuf->rwbuf(), pad, static_cast<size_t>(encbuf->size()));
    memcpy(encbuf->rwbuf(), in->rbuf(), static_cast<size_t>(in->size()));
    /* encrypt buffer */
    if(!gcry_cipher_encrypt(hd, encbuf->rwbuf(), static_cast<size_t>(encbuf->size()), nullptr, 0)) {
        LOG_CRITICAL("failed to encrypt data.");
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
    if(!gcry_cipher_decrypt(hd, decbuf.data(), static_cast<size_t>(decbuf.size()), nullptr, 0)) {
        LOG_CRITICAL("failed to get cipher tag.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* remove padding */
    decbuf.chop(decbuf.back());
    /* finalize cipher */
    if(!gcry_cipher_checktag(hd, tag.data(), static_cast<size_t>(tag.size()))) {
        LOG_CRITICAL("failed to get cipher tag.");
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
    CryptoBufferShPtr decbuf=CryptoBuffer::malloc(encbuf.size());
    memcpy(decbuf->rwbuf(), encbuf, static_cast<size_t>(decbuf->size()));
    if(!gcry_cipher_decrypt(hd, decbuf->rwbuf(), static_cast<size_t>(decbuf->size()), nullptr, 0)) {
        LOG_CRITICAL("failed to get cipher tag.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* remove padding */
    int pad=decbuf->rbuf()[decbuf->size()-1];
    CryptoBufferShPtr unpadded=CryptoBuffer::malloc(decbuf->size()-pad);
    memcpy(unpadded->rwbuf(), decbuf->rbuf(), static_cast<size_t>(decbuf->size()-pad));
    /* finalize cipher */
    if(!gcry_cipher_checktag(hd, tag.data(), static_cast<size_t>(tag.size()))) {
        LOG_CRITICAL("failed to get cipher tag.");
        gcry_cipher_close(hd);
        LOG_BOOL_RETURN(false);
    }
    /* success */
    out=unpadded;
    gcry_cipher_close(hd);
    LOG_BOOL_RETURN(true);
}
