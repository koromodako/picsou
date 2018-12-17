#include "crypto.h"

bool Crypto::init()
{

}

bool Crypto::term()
{

}

bool Crypto::init_wkey(const QString &pswd, QString &wkey)
{

}

bool Crypto::rewrap(const QString &prev_secret,
                    const QString &next_secret,
                    QString &wkey)
{

}

bool Crypto::encrypt(const QString &secret,
                     const QString &wkey,
                     const QByteArray &in,
                     QString &out)
{

}

bool Crypto::decrypt(const QString &secret,
                     const QString &wkey,
                     const QString &in,
                     QByteArray &out)
{

}
