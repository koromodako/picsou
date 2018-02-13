#ifndef QSECUREMEMORY_H
#define QSECUREMEMORY_H

#include <QByteArray>

class QSecureMemory
{
public:
    virtual ~QSecureMemory();
    QSecureMemory();
    QSecureMemory(size_t length);
    QSecureMemory(uchar *sbuf, size_t length);

    bool valid() const;
    void erase();
    bool resize(size_t length);

    inline uchar *data() { return _sbuf; }
    inline const uchar *const_data() const { return _sbuf; }
    inline size_t length() const { return _length; }

private:
    uchar *_sbuf;
    size_t _length;
};

#endif // QSECUREMEMORY_H
