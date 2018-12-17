#ifndef CRYPTO_H
#define CRYPTO_H


class Crypto
{
public:
    static bool init();
    static bool term();

    static bool encrypt();
    static bool decrypt();

private:
    Crypto();

};

#endif // CRYPTO_H
