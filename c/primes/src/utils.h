#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/// @brief Get the smaller number of the two. If they are the same, get the
/// first.
///
/// This will evaluate the smaller parameter twice.
#include <math.h>
#include <stddef.h>
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

/// @brief Get the larger number of the two. If they are the same, get the
/// first.
///
/// This will evaluate the larger parameter twice.
#define MAX(a, b) ((a) >= (b) ? (a) : (b))

/// @brief Estimates the number of primes below `n`.
static inline size_t est_prime_cnt(size_t n) {
    auto res = ceil((double)n / log((double)n));
    return (size_t)MAX(res, 0);
}

/// @brief Returns the upper estimate of nth prime.
static inline size_t est_nth_prime(size_t n) {
    double a = (double)n;
    double la = log(a);
    double ala = a * la;
    double lla = log(la);
    /// NOLINTNEXTLINE(readability-magic-numbers)
    double res = n < 7022 ? ala + a * lla : ala + a * (lla - 0.9385);
    res = ceil(res);
    return (size_t)MAX(0, res) | 1;
}

#endif // UTILS_H_INCLUDED