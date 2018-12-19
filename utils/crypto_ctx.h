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
#include <QSharedPointer>

class CryptoBuffer;
typedef QSharedPointer<CryptoBuffer> CryptoBufferShPtr;

class CryptoBuffer
{
public:
    enum RandomLevel {
        RANDOM_WEAK,
        RANDOM_STRONG,
        RANDOM_VERY_STRONG
    };

    virtual ~CryptoBuffer();
    CryptoBuffer(char *buf=nullptr, int size=0);

    static CryptoBufferShPtr malloc(int size);
    static CryptoBufferShPtr calloc(int mem_size, int mem_cnt);
    static CryptoBufferShPtr random(int size, RandomLevel lvl);
    static QByteArray random_ba(int size, RandomLevel lvl);

    inline bool null() const { return m_buf==nullptr; }
    inline int size() const { return m_size; }

    inline char *rwbuf() { return m_buf; }
    inline const char *rbuf() const { return m_buf; }

protected:
    Q_DISABLE_COPY(CryptoBuffer)

private:
    char *m_buf;
    int m_size;
};

class CryptoCtx
{
public:
    static bool lib_init();

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

    inline bool dpk_cached() const { return !m_dpk.isNull(); }
    inline bool salt_cached() const { return !m_salt.isNull(); }

    inline QString wkey() const { return m_wkey; }
    inline QString wsalt() const { return m_salt.toBase64(); }

protected:
    static bool lib_initialized();
    bool derive(const QString &pswd, const QByteArray &salt, CryptoBufferShPtr &key) const;
    bool encrypt(const CryptoBufferShPtr &key, const QByteArray &in, QString &out) const;
    bool encrypt(const CryptoBufferShPtr &key, const CryptoBufferShPtr &in, QString &out) const;
    bool decrypt(const CryptoBufferShPtr &key, const QString &in, QByteArray &out) const;
    bool decrypt(const CryptoBufferShPtr &key, const QString &in, CryptoBufferShPtr &out) const;

private:
    QString m_wkey; /* b64(iv+e(dpk)+tag) */
    CryptoBufferShPtr m_dpk;
    QByteArray m_salt;
};

#endif // CRYPTO_CTX_H
