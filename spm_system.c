#include <common.h>
#include <spm_system.h>
#include <stdio.h>
#include <stdlib.h>

s32 RANDOM_SEED = 0;

void __assert2(const char * filename, s32 line, const char * assertion, const char * message, ...) {
    char buf[1024];
    va_list args;
    va_start(args, message);
    vsprintf(buf, message, args);
    va_end(args);
    printf("[Error]\nAssertion \"%s\" failed in %s at line %d.\n%s", assertion, __FILE_NAME__, __LINE__, buf);
    return;
}

static s32 _rand_advance() {
    RANDOM_SEED = RANDOM_SEED * 0x5d588b65 + 1;
    return RANDOM_SEED;
}

static f32 _frand() {
    return irand(RAND_MAX) / (f32)RAND_MAX;
}

static s32 _rand(s32 max) // always inlined
{
    u32 divisor;
    u32 res;

    divisor = 0xffffffff;
    divisor /= (max + 1);
    if (divisor < 1)
        divisor = 1;

    do {
        res = _rand_advance() / divisor;
    } while (res >= max + 1);

    return (s32)res;
}

s32 rand() {
    return irand(RAND_MAX);
}

s32 irand(s32 max) {
    // This isn't converted back to being negative on return
    max = abs(max);

    if (max == 0) {
        // 0 will always result in zero
        return 0;
    } else if (max == 1) {
        // Special case for 1
        // 0-500 is 0, 501-1000 is 1
        if (_rand(1000) <= 500)
            return 0;
        else
            return 1;
    } else if (max == 100) {
        // Special case for 100
        // Selected from 0-1009 and divided by 10 (rounding down)
        return _rand(1009) / 10;
    } else {
        // Calculate value in requested range
        return _rand(max);
    }
}

f32 frand(f32 limit) {
    return limit * _frand();
}
