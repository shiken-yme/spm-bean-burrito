#pragma once

#include <common.h>
#include <stdarg.h>
#include <stdio.h>

#define RAND_MAX 0x7fff

void __assert2(const char * filename, s32 line, const char * assertion, const char * message, ...);

#define assert(condition, message)                                                                             \
    if (!(condition)) {                                                                                        \
        printf("Assertion \"%s\" failed in %s at line %d.\n%s", #condition, __FILE_NAME__, __LINE__, message); \
        abort();                                                                                               \
    }

#define assertf(condition, ...)                                      \
    if (!(condition)) {                                              \
        __assert2(__FILE_NAME__, __LINE__, #condition, __VA_ARGS__); \
        abort();                                                     \
    }

extern s32 RANDOM_SEED;

s32 rand();
s32 irand(s32 max);
f32 frand(f32 limit);
