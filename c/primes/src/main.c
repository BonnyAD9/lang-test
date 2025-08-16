#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "err.h"
#include "help.h"
#include "primap.h"

static void start(Args *args);
static bool m_default(Args *args);
static bool count(Args *args);
static bool nth(Args *args);
static int (*fast_is_prime_for(size_t start, size_t end))(
    Primap *pm, size_t n
);
static int is_prime(size_t num);

int main(int, char **argv) {
    auto ar = arg_parse(argv);
    start(&ar);
    if (err_any()) {
        err_print();
        return EXIT_FAILURE;
    }
}

static void start(Args *args) {
    switch (args->mode) {
    case MODE_ERROR:
        break;
    case MODE_HELP:
        help();
        break;
    case MODE_DEFAULT:
        m_default(args);
        break;
    case MODE_COUNT:
        count(args);
        break;
    case MODE_NTH:
        nth(args);
        break;
    }
}

static bool m_default(Args *args) {
    if (!args->ranged) {
        if (is_prime(args->end)) {
            printf("%zu is prime\n", args->end);
        } else {
            printf("%zu is not prime\n", args->end);
        }

        return true;
    }

    if (args->start <= 2 && args->end >= 2) {
        printf("2\n");
    }

    auto pm = pm_new();
    auto is = fast_is_prime_for(args->start, args->end);

    if (is == pm_is_prime && !pm_precalc(&pm, args->end - 1)) {
        pm_delete(&pm);
        return false;
    }

    for (size_t i = args->start | 1; i < args->end; i += 2) {
        switch (is(&pm, i)) {
        case -1:
            pm_delete(&pm);
            return false;
        case 1:
            printf("%zu\n", i);
            break;
        default:
            break;
        }
    }

    pm_delete(&pm);

    return true;
}

static bool count(Args *args) {
    if (!args->ranged) {
        if (is_prime(args->end)) {
            printf("1\n");
        } else {
            printf("0\n");
        }
        return true;
    }

    if (args->end == args->start) {
        printf("0\n");
        return true;
    }

    auto pm = pm_new();
    auto is = fast_is_prime_for(args->start, args->end);

    if (is == pm_is_prime && !pm_precalc(&pm, args->end - 1)) {
        pm_delete(&pm);
        return false;
    }

    size_t cnt = 0;

    for (size_t i = args->start | 1; i < args->end; i += 2) {
        cnt += is(&pm, i);
    }

    pm_delete(&pm);

    printf("%zu\n", cnt);

    return true;
}

static bool nth(Args *args) {
    auto pm = pm_new();
    if (!args->ranged) {
        auto res = pm_nth(&pm, args->end);
        pm_delete(&pm);
        if (res == 0) {
            return false;
        }

        printf("%zu\n", res);
        return true;
    }

    auto start = pm_nth(&pm, args->start);
    if (start == 0) {
        pm_delete(&pm);
        return false;
    }
    size_t cnt = 0;

    if (start == 2) {
        printf("2\n");
        ++cnt;
        ++start;
    }

    for (size_t i = start; cnt < args->end; i += 2) {
        switch (pm_is_prime(&pm, i)) {
        case -1:
            pm_delete(&pm);
            return false;
        case 1:
            printf("%zu\n", i);
            ++cnt;
            break;
        default:
            break;
        }
    }

    pm_delete(&pm);

    return true;
}

static int naive_is_prime(Primap *, size_t n) {
    return is_prime(n);
}

static int (*fast_is_prime_for(size_t start, size_t end))(
    Primap *pm, size_t n
) {
    constexpr double NAIVE_MULT = 0.2;
    auto sieve = (double)end * log((double)end);
    auto naive = (double)(end - start) * sqrt((double)end) * NAIVE_MULT;
    if (naive < sieve) {
        return naive_is_prime;
    }
    return pm_is_prime;
}

static int is_prime(size_t n) {
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
