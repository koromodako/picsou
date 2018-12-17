#ifndef CRYPTO_H
#define CRYPTO_H

#include <QString>

class Crypto
{
public:
    static bool init();
    static bool term();

    static bool init_wkey(const QString &pswd, QString &wkey);

    static bool rewrap(const QString &prev_secret,
                       const QString &next_secret,
                       QString &wkey);

    static bool encrypt(const QString &secret,
                        const QString &wkey,
                        const QByteArray &in,
                        QString &out);
    static bool decrypt(const QString &secret,
                        const QString &wkey,
                        const QString &in,
                        QByteArray &out);

private:
    Crypto() {}

};

#endif // CRYPTO_H
