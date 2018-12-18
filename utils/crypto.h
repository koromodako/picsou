#ifndef CRYPTO_H
#define CRYPTO_H

#include <QString>

class Crypto
{
public:
    /**
     * @brief init
     * @return
     */
    static bool init();
    /**
     * @brief init_wkey
     * @param pswd
     * @param wkey
     * @return
     */
    static bool init_wkey(const QString &pswd, QString &wkey);
    /**
     * @brief rewrap
     * @param prev_pswd
     * @param next_pswd
     * @param wkey
     * @return
     */
    static bool rewrap(const QString &prev_pswd,
                       const QString &next_pswd,
                       QString &wkey);
    /**
     * @brief encrypt
     * @param pswd
     * @param wkey
     * @param in
     * @param out
     * @return
     */
    static bool encrypt(const QString &pswd,
                        const QString &wkey,
                        const QByteArray &in,
                        QString &out);
    /**
     * @brief decrypt
     * @param secret
     * @param wkey
     * @param in
     * @param out
     * @return
     */
    static bool decrypt(const QString &pswd,
                        const QString &wkey,
                        const QString &in,
                        QByteArray &out);

private:
    Crypto() {}

};

#endif // CRYPTO_H
