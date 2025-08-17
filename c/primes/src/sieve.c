#include "sieve.h"

#include <assert.h>
#include <stdbit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "primes.h"

static bool resize(Sieve *pm, size_t n);
static void extend(Sieve *pm, size_t old);

constexpr static size_t FIRST64 =
    //  12   11   10    9    8    7    6    5    4    3    2    1    0
    // 7531975319753197531975319753197531975319753197531975319753197531
    0b1000000101101101000100101001101001100100101101001100101101101110;

Sieve es_new() {
    return (Sieve){
        .map = nullptr,
        .len = 0,
    };
}

int es_is_prime(Sieve *pm, size_t n) {
    if (n % 2 == 0) {
        return n == 2;
    }
    n /= 2;
    const auto idx = n / SIZE_WIDTH;
    const auto mask = (size_t)1 << (n % SIZE_WIDTH);

    if (idx < pm->len) {
        return (pm->map[idx] & mask) == mask;
    }

    if (!resize(pm, stdc_bit_ceil(idx + 1))) {
        return -1;
    }

    return (pm->map[idx] & mask) == mask;
}

void es_delete(Sieve *pm) {
    free(pm->map);
    *pm = es_new();
}

size_t es_nth(Sieve *pm, size_t n) {
    if (n == 0) {
        return 2;
    }
    // Fast precalculation
    es_is_prime(pm, est_nth_prime(n));

    size_t i = 0;

    for (size_t c = 0; c < n; ++i) {
        switch (es_is_prime(pm, i)) {
        case -1:
            return 0;
        case 0:
            break;
        case 1:
            ++c;
            break;
        default:
            assert(false);
            break;
        }
    }

    return i - 1;
}

bool es_precalc(Sieve *pm, size_t n) {
    auto size = n / 2 / SIZE_WIDTH;
    if (size > pm->len) {
        return resize(pm, size + 1);
    }
    return true;
}

static bool resize(Sieve *pm, size_t n) {
    auto new_map = (size_t *)realloc(pm->map, n * sizeof(*pm->map));
    if (!new_map) {
        err_c(STR("Failed to resize primap."));
        return false;
    }

    auto old_len = pm->len;
    pm->len = n;
    pm->map = new_map;

    memset(pm->map + old_len, UINT8_MAX, (n - old_len) * sizeof(*pm->map));

    extend(pm, old_len);
    return true;
}

static void extend(Sieve *pm, size_t old) {
    size_t i = old * 64;
    if (old == 0) {
        *pm->map = FIRST64;
        // NOLINTNEXTLINE(misc-redundant-expression)
        if (SIZE_WIDTH > 64) {
            i = 64;
        } else {
            i = SIZE_WIDTH;
            if (pm->len == 1) {
                return;
            }
        }
    }

    auto lim = pm->len * SIZE_WIDTH;

    size_t j = 1;
    for (; j < i; ++j) {
        auto idx = j / SIZE_WIDTH;
        auto mask = (size_t)1 << (j % SIZE_WIDTH);
        if ((pm->map[idx] & mask) == 0) {
            continue;
        }

        auto k = (j * j + j) * 2;
        auto j2 = j * 2 + 1;
        if (k >= i) {
            break;
        }

        auto i2 = i * 2;
        k = i2 + j2 - i2 % j2;
        if (k % 2 == 0) {
            k += j2;
        }
        k /= 2;

        for (; k < lim; k += j2) {
            auto idx = k / SIZE_WIDTH;
            auto mask = (size_t)1 << (k % SIZE_WIDTH);

            pm->map[idx] &= ~mask;
        }
    }

    for (i = j; i < lim; ++i) {
        auto idx = i / SIZE_WIDTH;
        auto mask = (size_t)1 << (i % SIZE_WIDTH);
        if ((pm->map[idx] & mask) == 0) {
            continue;
        }

        auto k = (i * i + i) * 2;
        auto i2 = i * 2 + 1;
        if (k >= lim) {
            return;
        }

        for (; k < lim; k += i2) {
            auto idx = k / SIZE_WIDTH;
            auto mask = (size_t)1 << (k % SIZE_WIDTH);
            pm->map[idx] &= ~mask;
        }
    }
}
