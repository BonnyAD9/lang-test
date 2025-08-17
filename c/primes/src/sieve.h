#ifndef PRIMAP_H_INCLUDED
#define PRIMAP_H_INCLUDED

#include <stddef.h>

/// Expandable erastothenes sieve.
typedef struct {
    size_t *map;
    size_t len;
} Sieve;

/// @brief Create new erastothenes sieve struct.
///
/// This should be deleted with [`pm_delete`].
/// @returns New instance of erastothenes sieve.
Sieve es_new();

/// @brief Checks whether `n` is prime.
/// @param pm Erastothenes sieve which will be used for the check. It may be
/// expanded.
/// @param n Number which will be checked for primeness.
/// @returrns
/// - `0` if `n` is not prim
/// - `1` if `n` is prime
/// - `-1` on error and `err` is populated with error.
int es_is_prime(Sieve *pm, size_t n);

/// @brief Frees memory allocated by `pm`.
/// @param pm Erastothenes sieve which memory will be freed.
void es_delete(Sieve *pm);

/// @brief Get the `n`th prime number.
/// @param pm Erastothenes sieve which will be used to find the prime. It may
/// be expanded.
/// @param n Index of prime to find. For `n = 1` this will return `2`.
/// @returns `n`th prime. On error returns `0` and `err` is populated.
size_t es_nth(Sieve *pm, size_t n);

/// @brief Precalculate erastothenes sieve for numbers up to `n`.
/// @param pm Erastothenes sieve which will be expanded.
/// @param n Largest number that can be directly checked after precalculation.
/// @returns `true` on success, `false` on failure and `err` is populated.
bool es_precalc(Sieve *pm, size_t n);

#endif // PRIMAP_H_INCLUDED