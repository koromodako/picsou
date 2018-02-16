#ifndef QCRYPTOWRAPPER_H
#define QCRYPTOWRAPPER_H

#include <QString>

#ifndef USE_WIN_CRYPTO_API
typedef uint gcry_error_t;
#else
#   error   not implemented !
#endif

class QCryptoWrapper
{
public:
    virtual ~QCryptoWrapper();
    QCryptoWrapper();

    bool wrap(gcry_error_t err);

    inline QString err_source() { return _latest_error_source; }
    inline QString err_description() { return _latest_error_description; }

private:
    QString _latest_error_source;
    QString _latest_error_description;
};

#endif // QCRYPTOWRAPPER_H
