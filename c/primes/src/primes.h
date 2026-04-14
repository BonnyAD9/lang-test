#ifndef PRIMES_H_INCLUDED
#define PRIMES_H_INCLUDED

#include <stddef.h>

#include "sieve.h"

typedef struct {
    size_t num;
    size_t idx;
} FactorState;

/// @brief Get fast function for checking all numbers for primeness within the
/// given range.
/// @param start Start of the range.
/// @param end End of the range.
/// @returns Optimal function for checking primeness of number.
int (*fast_is_prime_for(size_t start, size_t end))(Sieve *pm, size_t n);

/// @brief Fast check whether `num` is prime.
/// @param num Number to check whether is prime.
int is_prime(size_t num);

/// @brief Estimates the number of primes below `n`.
/// @param n Upper bound for the estimate.
/// @returns Estimate for the number of primes below `n`.
size_t est_prime_cnt(size_t n);

/// @brief Estimates value of `n`th prime.
/// @param n Index of the estimated prime.
/// @returns Upper estimate for `n`.
size_t est_nth_prime(size_t n);

/// @brief Initialize next factor state for calculating prime factors.
/// @param n Number for which the factors will be calculated.
FactorState factors_init(size_t n);

/// @brief Calcualtes the next prime factor with the given state.
/// @param fs The state of the factor calculation.
/// @returns The next factor. 0 if there is no next factor.
size_t next_factor(FactorState *fs);

#endif // PRIMES_H_INCLUDED