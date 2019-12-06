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
#include <botan/aead.h>
#include <botan/version.h>
#include <botan/pwdhash.h>
#include <botan/exceptn.h>
#include <botan/system_rng.h>

/*
 * Warning:
 *      Picsou attempts to reduce presence of secrets in memory (and disk through swap/hibernation) yet
 *      it does not guarantees that secrets cannot be retrieved through memory dumping, hibernation file carving, etc.
 */

typedef std::unique_ptr<Botan::PasswordHashFamily> PasswordHashFamilyPtr;
typedef std::unique_ptr<Botan::PasswordHash> PasswordHashPtr;
typedef std::unique_ptr<Botan::AEAD_Mode> AEADModePtr;

static const int CRY_IV_SIZE=16;
static const int CRY_KEY_SIZE=16;
static const int CRY_SALT_SIZE=32;
static const size_t CRY_PBKDF_ITER_COUNT=1000;
static const std::string CRY_PBKDF_FUNC="PBKDF2(SHA-256)";
static const std::string CRY_AEAD_MODE="AES-128/GCM";

#define CHECK_SALT_CACHED() \
    do { \
        if(!salt_cached()) { \
            LOG_CRITICAL("operation requires salt to be cached.") \
            LOG_BOOL_RETURN(false) \
        } \
    } while(0)

#define CHECK_DPK_CACHED() \
    do { \
        if(!dpk_cached()) { \
            LOG_CRITICAL("operation requires dpk to be cached.") \
            LOG_BOOL_RETURN(false) \
        } \
    } while(0)

QString CryptoCtx::lib_description()
{
    /* Expected to be "LIBNAME LIBVERSION" */
    return QString("Botan %0.%1.%2").arg(QString::number(Botan::version_major()),
                                         QString::number(Botan::version_minor()),
                                         QString::number(Botan::version_patch()));
}

QByteArray CryptoCtx::cb2ba(const CryptoBuf &cb)
{
    return QByteArray(reinterpret_cast<const char*>(cb.data()), static_cast<int>(cb.size()));
}

CryptoBuf CryptoCtx::ba2cb(const QByteArray &ba)
{
    return CryptoBuf(ba.begin(), ba.end());
}

CryptoBuf CryptoCtx::rand_secure(size_t size)
{
    Botan::System_RNG srng;
    return srng.random_vec(size);
}

QByteArray CryptoCtx::rand(size_t size)
{
    Botan::System_RNG srng;
    CryptoBuf rbuf=srng.random_vec(size);
    return cb2ba(rbuf);
}

CryptoCtx::CryptoCtx(const QString &wkey,
                     const QString &wsalt) :
    m_wkey(wkey),
    m_dpk(),
    m_salt()
{
    if(!wsalt.isNull()) {
        m_salt=QByteArray::fromBase64(wsalt.toUtf8());
    }
}

bool CryptoCtx::init(const QString &pswd)
{
    LOG_IN("pswd")
    /* if salt is already cached something is wrong */
    if(salt_cached()) {
        LOG_CRITICAL("crypto context has been initialized already.")
        LOG_BOOL_RETURN(false)
    }
    /* generate new salt */
    QByteArray salt=rand(CRY_SALT_SIZE);
    CryptoBuf mk;
    if(!derive(pswd, salt, mk)) {
        LOG_CRITICAL("failed to derive MK.")
        LOG_BOOL_RETURN(false)
    }
    /* generate new DPK */
    CryptoBuf dpk=rand_secure(CRY_KEY_SIZE);
    if(!encrypt(mk, dpk, m_wkey)) {
        LOG_CRITICAL("failed to wrap DPK.")
        LOG_BOOL_RETURN(false)
    }
    /* success */
    m_salt=salt;
    m_dpk=dpk;
    LOG_BOOL_RETURN(true)
}

bool CryptoCtx::wrap(const QByteArray &cdata, QString &wdata) const
{
    LOG_IN("cdata,wdata")
    CHECK_SALT_CACHED();
    CHECK_DPK_CACHED();
    /* encrypt given data */
    if(!encrypt(m_dpk, cdata, wdata)) {
        LOG_CRITICAL("data encryption failed.")
        LOG_BOOL_RETURN(false)
    }
    LOG_BOOL_RETURN(true)
}

bool CryptoCtx::unwrap(const QString &pswd, const QString &wdata, QByteArray &cdata)
{
    LOG_IN("pswd,wdata,cdata")
    CHECK_SALT_CACHED();
    /* if DPK is already cached something is wrong */
    if(dpk_cached()) {
        LOG_CRITICAL("dpk is already cached meaning the object has been unwrapped already.")
        LOG_BOOL_RETURN(false)
    }
    /* retrieve MK */
    CryptoBuf mk;
    if(!derive(pswd, m_salt, mk)) {
        LOG_CRITICAL("failed to derive MK.")
        LOG_BOOL_RETURN(false)
    }
    /* decrypt DPK with MK */
    CryptoBuf dpk;
    if(!decrypt(mk, m_wkey, dpk)) {
        LOG_CRITICAL("invalid user password.")
        LOG_BOOL_RETURN(false)
    }
    /* decrypt data with DPK */
    if(!decrypt(dpk, wdata, cdata)) {
        LOG_CRITICAL("failed to decrypt data.")
        LOG_BOOL_RETURN(false)
    }
    m_dpk=dpk;
    LOG_BOOL_RETURN(true)
}

bool CryptoCtx::rewrap(const QString &prev_pswd, const QString &next_pswd)
{
    LOG_IN("prev_pswd,next_pswd")
    CHECK_SALT_CACHED();
    /* compute current MK */
    CryptoBuf prev_mk;
    if(!derive(prev_pswd, m_salt, prev_mk)) {
        LOG_CRITICAL("failed to derive previous MK.")
        LOG_BOOL_RETURN(false)
    }
    /* decrypt DPK using current MK */
    CryptoBuf dpk;
    if(!decrypt(prev_mk, m_wkey, dpk)) {
        LOG_CRITICAL("invalid user password.")
        LOG_BOOL_RETURN(false)
    }
    /* compute next MK */
    CryptoBuf next_mk;
    if(!derive(next_pswd, m_salt, next_mk)) {
        LOG_CRITICAL("failed to derive next MK.")
        LOG_BOOL_RETURN(false)
    }
    /* encrypt DPK with next MK */
    if(!encrypt(next_mk, dpk, m_wkey)) {
        LOG_CRITICAL("failed to wrap DPK.")
        LOG_BOOL_RETURN(false)
    }
    LOG_INFO("rewraping succeeded.")
    LOG_BOOL_RETURN(true)
}

bool CryptoCtx::derive(const QString &pswd, const QByteArray &salt, CryptoBuf &out) const
{
    LOG_IN("pswd,salt,out")
    QByteArray pswdba=pswd.toUtf8();
    PasswordHashFamilyPtr phf=Botan::PasswordHashFamily::create(CRY_PBKDF_FUNC);
    if(!phf) {
        LOG_CRITICAL("failed to create PasswordHashFamily instance.")
        LOG_BOOL_RETURN(false)
    }
    PasswordHashPtr pwdhash=phf->from_params(CRY_PBKDF_ITER_COUNT);
    CryptoBuf key(CRY_KEY_SIZE, 0);
    pwdhash->derive_key(key.data(), key.size(),
                        pswdba.data(), static_cast<size_t>(pswdba.size()),
                        reinterpret_cast<const uint8_t*>(salt.data()), static_cast<size_t>(salt.size()));
    out=key;
    LOG_BOOL_RETURN(true)
}

bool CryptoCtx::encrypt(const CryptoBuf &key, const QByteArray &in, QString &out) const
{
    LOG_IN("key,in")
    LOG_BOOL_RETURN(encrypt(key, ba2cb(in), out))
}

bool CryptoCtx::encrypt(const CryptoBuf &key, const CryptoBuf &in, QString &out) const
{
    LOG_IN("key,in")
    /* generate random iv */
    CryptoBuf iv=rand_secure(CRY_IV_SIZE);
    /* encrypt */
    AEADModePtr cipher=Botan::AEAD_Mode::create(CRY_AEAD_MODE, Botan::ENCRYPTION);
    if(!cipher) {
        LOG_CRITICAL("failed to instanciate AEAD_Mode.")
        LOG_BOOL_RETURN(false)
    }
    cipher->set_key(key);
    cipher->start(iv);
    CryptoBuf encbuf=in;
    cipher->finish(encbuf);
    /* pack data */
    QByteArray packed;
    packed+=cb2ba(iv);
    packed+=cb2ba(encbuf);
    out=packed.toBase64();
    LOG_BOOL_RETURN(true)
}

bool CryptoCtx::decrypt(const CryptoBuf &key, const QString &in, QByteArray &out) const
{
    LOG_IN("key,in,out")
    CryptoBuf plain;
    if(!decrypt(key, in, plain)) {
        LOG_BOOL_RETURN(false)
    }
    out=cb2ba(plain);
    LOG_BOOL_RETURN(true)
}

bool CryptoCtx::decrypt(const CryptoBuf &key, const QString &in, CryptoBuf &out) const
{
    LOG_IN("key,in,out")
    /* unpack data */
    QByteArray encbuf=QByteArray::fromBase64(in.toUtf8());
    QByteArray iv(encbuf.data(), CRY_IV_SIZE);
    QByteArray data=encbuf.mid(CRY_IV_SIZE, encbuf.size()-CRY_IV_SIZE);
    /* decrypt */
    AEADModePtr cipher=Botan::AEAD_Mode::create(CRY_AEAD_MODE, Botan::DECRYPTION);
    if(!cipher) {
        LOG_CRITICAL("failed to instanciate AEAD_Mode.")
        LOG_BOOL_RETURN(false)
    }
    cipher->set_key(key);
    cipher->start(ba2cb(iv));
    CryptoBuf decbuf=ba2cb(data);
    try {
        cipher->finish(decbuf);
    } catch (Botan::Integrity_Failure&) {
        LOG_CRITICAL("failed to decrypt data.")
        LOG_BOOL_RETURN(false)
    }
    /* success */
    out=decbuf;
    LOG_BOOL_RETURN(true)
}
