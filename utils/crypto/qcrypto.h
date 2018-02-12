#ifndef QCRYPTO_H
#define QCRYPTO_H

#include <QByteArray>

class QCrypto
{
public:
    /**
     * @brief The RandomnessLevel enum
     */






public:
    virtual ~QCrypto();

    /*
     * Singleton methods
     */
    /**
     * @brief initialize
     * @return
     */
    static bool initialize();
    /**
     * @brief instance
     * @return
     */
    static QCrypto *instance();
    /**
     * @brief terminate
     */
    static void terminate();

private:
    explicit QCrypto();
};

#endif // QCRYPTO_H
