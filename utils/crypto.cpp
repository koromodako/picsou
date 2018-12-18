#include "crypto.h"

bool Crypto::init()
{

}

bool Crypto::init_wkey(const QString &pswd, QString &wkey)
{

}

bool Crypto::rewrap(const QString &prev_pswd,
                    const QString &next_pswd,
                    QString &wkey)
{

}

bool Crypto::encrypt(const QString &pswd,
                     const QString &wkey,
                     const QByteArray &in,
                     QString &out)
{

}

bool Crypto::decrypt(const QString &pswd,
                     const QString &wkey,
                     const QString &in,
                     QByteArray &out)
{

}
