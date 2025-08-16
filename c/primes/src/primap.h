#ifndef PRIMAP_H_INCLUDED
#define PRIMAP_H_INCLUDED

#include <stddef.h>

typedef struct {
    size_t *map;
    size_t len;
} Primap;

Primap pm_new();

// 0 or 1 on success
// negative on error
int pm_is_prime(Primap *pm, size_t n);

void pm_delete(Primap *pm);

size_t pm_nth(Primap *pm, size_t n);

bool pm_precalc(Primap *pm, size_t n);

#endif // PRIMAP_H_INCLUDED