#include "primap.h"

#include <math.h>
#include <stdbit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "err.h"

static bool resize(Primap *pm, size_t n);
static void extend(Primap *pm, size_t old);

constexpr size_t FIRST64 =
    //  12   11   10    9    8    7    6    5    4    3    2    1    0
    //7531975319753197531975319753197531975319753197531975319753197531
    0b1000000101101101000100101001101001100100101101001100101101101110;

Primap pm_new() {
    return (Primap){
        .map = nullptr,
        .len = 0,
    };
}

int pm_is_prime(Primap *pm, size_t n) {
    if (n % 2 == 0) {
        return n == 2;
    }
    n /= 2;
    const auto idx = n / SIZE_WIDTH;
    const auto mask = (size_t)1 << (n % SIZE_WIDTH);

    if (idx < pm->len) {
        return (pm->map[idx] & mask) == mask;
    }

    if (!resize(pm, idx + 1)) {
        return -1;
    }

    return (pm->map[idx] & mask) == mask;
}

void pm_delete(Primap *pm) {
    free(pm->map);
    *pm = pm_new();
}

size_t pm_nth(Primap *pm, size_t n) {
    // Fast precalculation
    pm_is_prime(pm, n * (size_t)log(n));
    
    size_t i = 0;

    for (size_t c = 0; c < n; ++i) {
        switch (pm_is_prime(pm, i)) {
        case -1:
            return 0;
        case 0:
            break;
        case 1:
            ++c;
            break;
        }
    }

    return i - 1;
}

static bool resize(Primap *pm, size_t n) {
    const auto new_len = stdc_bit_ceil(n);

    auto new_map = (size_t *)realloc(pm->map, new_len * sizeof(*pm->map));
    if (!new_map) {
        err_c(STR("Failed to resize primap."));
        return false;
    }

    auto old_len = pm->len;
    pm->len = new_len;
    pm->map = new_map;

    memset(pm->map + old_len, 0xFF, (new_len - old_len) * sizeof(*pm->map));

    extend(pm, old_len);
    return true;
}

static void extend(Primap *pm, size_t old) {
    size_t i = old * 64;
    if (old == 0) {
        *pm->map = FIRST64;
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