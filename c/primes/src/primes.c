#include "primes.h"

#include <math.h>

#include "utils.h"

static int naive_is_prime(Sieve *, size_t n) {
    return is_prime(n);
}

int (*fast_is_prime_for(size_t start, size_t end))(Sieve *pm, size_t n) {
    constexpr double NAIVE_MULT = 0.2;
    auto sieve = (double)end * log((double)end);
    auto naive = (double)(end - start) * sqrt((double)end) * NAIVE_MULT;
    if (naive < sieve) {
        return naive_is_prime;
    }
    return es_is_prime;
}

int is_prime(size_t n) {
    if (n <= 1) {
        return false;
    }

    if (n == 2 || n == 3) {
        return true;
    }

    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }

    auto lim = (size_t)ceil(sqrt((double)n));

    // NOLINTNEXTLINE(readability-magic-numbers)
    for (size_t i = 5; i <= lim; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

size_t est_prime_cnt(size_t n) {
    auto res = ceil((double)n / log((double)n));
    return (size_t)MAX(res, 0);
}

size_t est_nth_prime(size_t n) {
    double a = (double)n;
    double la = log(a);
    double ala = a * la;
    double lla = log(la);
    // NOLINTNEXTLINE(readability-magic-numbers)
    double res = n < 7022 ? ala + a * lla : ala + a * (lla - 0.9385);
    res = ceil(res);
    return (size_t)MAX(0, res) | 1;
}

FactorState factors_init(size_t n) {
    return (FactorState){
        .num = n,
        .idx = 0,
    };
}

size_t next_factor(FactorState *fs) {
    size_t n = fs->num;
    size_t i = fs->idx;

    if (n == 1) {
        return 0;
    }

    if (i == 0) {
        if (n % 2 == 0) {
            fs->num = n / 2;
            return 2;
        }

        if (n % 3 == 0) {
            fs->num = n / 3;
            return 3;
        }

        // NOLINTNEXTLINE(readability-magic-numbers)
        i = 5;
    }

    auto lim = (size_t)ceil(sqrt((double)n));

    // NOLINTNEXTLINE(readability-magic-numbers)
    for (; i <= lim; i += 6) {
        if (n % i == 0) {
            fs->num = n / i;
            fs->idx = i;
            return i;
        }

        auto ii = i + 2;
        if (n % ii == 0) {
            fs->num = n / ii;
            fs->idx = i;
            return ii;
        }
    }

    fs->num = 1;
    fs->idx = 1;
    return n;
}
