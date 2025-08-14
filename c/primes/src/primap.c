#include "primap.h"

#include <stdbit.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool resize(Primap *pm, size_t n);
static void extend(Primap *pm, size_t old);

constexpr size_t FIRST64 =
    // 6         5         4         3         2         1         0
    //10987654321098765432109876543210987654321098765432109876543210
    0b10100000100000100010100010000010100000100010100010100010101100;

Primap pm_new() {
    return (Primap){
        .map = nullptr,
        .len = 0,
    };
}

int pm_is_prime(Primap *pm, size_t n) {
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
    // TODO: fast precalculation
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
        return false;
    }

    auto old_len = pm->len;
    pm->len = new_len;
    pm->map = new_map;

    memset(pm->map + old_len, 0xAA, (new_len - old_len) * sizeof(*pm->map));

    extend(pm, old_len);
    return true;
}

static void extend(Primap *pm, size_t old) {
    size_t i = old * 64;
    if (old == 0) {
        *pm->map = FIRST64;
        i = SIZE_WIDTH > 64 ? 64 : SIZE_WIDTH;
    }

    auto lim = pm->len * SIZE_WIDTH;

    for (size_t j = 3; j < i; ++j) {
        if (!pm_is_prime(pm, j)) {
            continue;
        }

        auto add = j * 2;
        auto k = j * j;
        if (k < i) {
            auto d = i / j;
            auto m = i % j;
            k = i + j - m;
            if (d & 1) {
                k += j;
            }
        }

        for (; k < lim; k += add) {
            auto idx = k / SIZE_WIDTH;
            auto mask = (size_t)1 << (k % SIZE_WIDTH);

            pm->map[idx] &= ~mask;
        }
    }

    for (; i < lim; ++i) {
        if (!pm_is_prime(pm, i)) {
            continue;
        }

        auto add = i * 2;
        for (auto k = i * i; k < lim; k += add) {
            auto idx = k / SIZE_WIDTH;
            auto mask = (size_t)1 << (k % SIZE_WIDTH);
            pm->map[idx] &= ~mask;
        }
    }
}