#include "qrandombytes.h"
#include "utils/macro.h"
#include <gcrypt.h>

QRandomBytes::~QRandomBytes()
{

}

QRandomBytes::QRandomBytes(size_t length, RandomnessLevel level) :
    QSecureMemory(length)
{
    gcry_random_level_t lvl;
    switch (level) {
        case WEAK_RANDOM: lvl=GCRY_WEAK_RANDOM; break;
        case STRONG_RANDOM: lvl=GCRY_STRONG_RANDOM; break;
        case VERY_STRONG_RANDOM: lvl=GCRY_VERY_STRONG_RANDOM; break;
    }
    gcry_randomize(data(), this->length(), lvl);
}

