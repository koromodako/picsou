#ifndef QNONCE_H
#define QNONCE_H

#include "qsecurememory.h"

class QNonce : public QSecureMemory
{
public:
    virtual ~QNonce();
    QNonce(size_t length);

private:
    unsigned char *_d;
};

#endif // QNONCE_H
