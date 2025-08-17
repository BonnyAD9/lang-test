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
    /// NOLINTNEXTLINE(readability-magic-numbers)
    double res = n < 7022 ? ala + a * lla : ala + a * (lla - 0.9385);
    res = ceil(res);
    return (size_t)MAX(0, res) | 1;
}