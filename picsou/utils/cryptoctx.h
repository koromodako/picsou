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
#ifndef CRYPTO_CTX_H
#define CRYPTO_CTX_H

#include <QString>
#include <botan/secmem.h>

typedef Botan::secure_vector<quint8> CryptoBuf;

class CryptoCtx
{
public:
    /* underlying library information */
    static QString lib_description();
    /* type conversion */
    static QByteArray cb2ba(const CryptoBuf &cb);
    static CryptoBuf ba2cb(const QByteArray &ba);
    /* crypto-compatible random buffers */
    static CryptoBuf rand_secure(size_t size);
    static QByteArray rand(size_t size);
    /**
     * @brief Creates a new CryptoCtx caching wkey, wdat and wsalt
     * @details
     *      1. Loads given wsalt into m_salt
     * @param wkey Base64 wrapped encrypted DPK
     * @param wdat Base64 wrapped encrypted data
     * @param wsalt Base64 wrapped salt
     */
    CryptoCtx(const QString &wkey = QString(),
              const QString &wsalt = QString());
    /**
     * @brief Initilizes a CryptoCtx with given user password
     * @details
     *      1. Generates m_salt
     *      2. Sets m_wsalt
     *      2. Derives given password using m_salt to get MK
     *      3. Generates a new DPK and store it in m_dpk
     *      4. Encrypts DPK with MK and store result in m_wkey
     *      Note:
     *          Context is considered unwrapped
     * @param pswd User password
     * @return
     */
    bool init(const QString &pswd);
    /**
     * @brief Wraps given cdata into wdata using cached DPK (m_dpk)
     * @warning Requires dpk to be cached, meaning unwrap() must have been called before.
     * @param cdata Clear data to be encrypted
     * @param wdata Wrapped data to be stored on disk
     * @return
     */
    bool wrap(const QByteArray &cdata, QString &wdata) const;
    /**
     * @brief Unwraps given wdata into cdata using given password to unwrap DPK
     * @warning Requires salt to be cached, meaning
     *      1. init() must have been called
     *      2. OR CryptoCtx built with a valid wsalt string
     * @param pswd  User password
     * @param wdata Wrapped data stored on disk
     * @param cdata Clear data to be used in memory
     * @return
     */
    bool unwrap(const QString &pswd, const QString &wdata, QByteArray &cdata);
    /**
     * @brief Rewraps DPK
     * @param prev_pswd Previous user password
     * @param next_pswd New user password
     * @return
     */
    bool rewrap(const QString &prev_pswd, const QString &next_pswd);

    inline bool dpk_cached() const { return !m_dpk.empty(); }
    inline bool salt_cached() const { return !m_salt.isNull(); }

    inline QString wkey() const { return m_wkey; }
    inline QString wsalt() const { return m_salt.toBase64(); }

protected:
    bool derive(const QString &pswd, const QByteArray &salt, CryptoBuf &out) const;
    bool encrypt(const CryptoBuf &key, const QByteArray &in, QString &out) const;
    bool encrypt(const CryptoBuf &key, const CryptoBuf &in, QString &out) const;
    bool decrypt(const CryptoBuf &key, const QString &in, QByteArray &out) const;
    bool decrypt(const CryptoBuf &key, const QString &in, CryptoBuf &out) const;

private:
    QString m_wkey; /* b64(iv+e(dpk)+tag) */
    CryptoBuf m_dpk;
    QByteArray m_salt;
};

#endif // CRYPTO_CTX_H
