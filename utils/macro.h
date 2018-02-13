#ifndef MACRO_H
#define MACRO_H

#define IS_FLAG_SET(field, flag) (((field)&(flag))==(flag))
#define GCRYPT_FAILED(func, ...) ((func(__VA_ARGS__))!=0)

#endif // MACRO_H
