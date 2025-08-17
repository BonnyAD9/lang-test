#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "err.h"
#include "help.h"
#include "primes.h"
#include "sieve.h"
#include "str.h"
#include "term.h"

static void start(Args *args);
static bool t_default(Args *args);
static bool default_single(Args *args);
static bool default_ranged(Args *args);
static bool default_estimate(Args *args);
static bool count(Args *args);
static bool count_single(Args *args);
static bool count_ranged(Args *args);
static bool count_estimate(Args *args);
static bool nth(Args *args);
static bool nth_single(Args *args);
static bool nth_ranged(Args *args);
static bool nth_estimate(Args *args);

int main(int, char **argv) {
    auto ar = arg_parse((const char *const *)argv);
    start(&ar);
    arg_delete(&ar);
    if (err_any()) {
        err_print();
        return EXIT_FAILURE;
    }
}

static void start(Args *args) {
    switch (args->type) {
    case TYPE_ERROR:
        break;
    case TYPE_HELP:
        help();
        break;
    case TYPE_DEFAULT:
        t_default(args);
        break;
    case TYPE_COUNT:
        count(args);
        break;
    case TYPE_NTH:
        nth(args);
        break;
    }
}

static bool t_default(Args *args) {
    switch (args->mode) {
    case MODE_SINGLE:
        return default_single(args);
    case MODE_RANGED:
        return default_ranged(args);
    case MODE_ESTIMATE:
        return default_estimate(args);
    }
}

static bool default_single(Args *args) {
    if (is_prime(args->end)) {
        if (fprintf(args->out, "%zu is prime\n", args->end) < 0) {
            err_c(STR("Failed to write to file."));
            return false;
        }
    } else {
        if (fprintf(args->out, "%zu is not prime\n", args->end) < 0) {
            err_c(STR("Failed to write to file."));
            return false;
        }
    }

    return true;
}

static bool default_ranged(Args *args) {
    if (args->start <= 2 && args->end >= 2) {
        if (fprintf(args->out, "2\n") < 0) {
            err_c(STR("Failed to write to file."));
            return false;
        }
    }

    auto pm = es_new();
    auto is = fast_is_prime_for(args->start, args->end);

    if (is == es_is_prime && !es_precalc(&pm, args->end - 1)) {
        es_delete(&pm);
        return false;
    }

    for (size_t i = args->start | 1; i < args->end; i += 2) {
        switch (is(&pm, i)) {
        case -1:
            es_delete(&pm);
            return false;
        case 1:
            if (fprintf(args->out, "%zu\n", i) < 0) {
                err_c(STR("Failed to write to file."));
                return false;
            }
            break;
        default:
            break;
        }
    }

    es_delete(&pm);

    return true;
}

static bool default_estimate(Args *) {
    err(STR("Estimate not available for checking if number is prime."));
    return false;
}

static bool count(Args *args) {
    switch (args->mode) {
    case MODE_SINGLE:
        return count_single(args);
    case MODE_RANGED:
        return count_ranged(args);
    case MODE_ESTIMATE:
        return count_estimate(args);
    }
}

static bool count_single(Args *args) {
    if (is_prime(args->end)) {
        if (fprintf(args->out, "1\n") < 0) {
            err_c(STR("Failed to write to file."));
            return false;
        }
    } else {
        if (fprintf(args->out, "0\n") < 0) {
            err_c(STR("Failed to write to file."));
            return false;
        }
    }
    return true;
}

static bool count_ranged(Args *args) {
    auto pm = es_new();
    auto is = fast_is_prime_for(args->start, args->end);

    if (is == es_is_prime && !es_precalc(&pm, args->end - 1)) {
        es_delete(&pm);
        return false;
    }

    size_t cnt = 0;

    for (size_t i = args->start | 1; i < args->end; i += 2) {
        cnt += is(&pm, i);
    }

    es_delete(&pm);

    if (fprintf(args->out, T_MOVE_TO_START T_ERASE_TO_END "%zu\n", cnt) < 0) {
        err_c(STR("Failed to write to file."));
        return false;
    }

    return true;
}

static bool count_estimate(Args *args) {
    auto se = est_prime_cnt(args->start);
    auto ee = est_prime_cnt(args->end);
    if (fprintf(args->out, "%zu\n", ee - se) < 0) {
        err_c(STR("Failed to write to file."));
        return false;
    }
    return true;
}

static bool nth(Args *args) {
    switch (args->mode) {
    case MODE_SINGLE:
        return nth_single(args);
    case MODE_RANGED:
        return nth_ranged(args);
    case MODE_ESTIMATE:
        return nth_estimate(args);
    }
}

static bool nth_single(Args *args) {
    auto pm = es_new();
    auto res = es_nth(&pm, args->end);
    es_delete(&pm);
    if (res == 0) {
        return false;
    }

    if (fprintf(args->out, "%zu\n", res) < 0) {
        err_c(STR("Failed to write to file."));
        return false;
    }
    return true;
}

static bool nth_ranged(Args *args) {
    auto pm = es_new();

    auto start = es_nth(&pm, args->start);
    if (start == 0) {
        es_delete(&pm);
        return false;
    }
    size_t cnt = 0;

    if (start == 2) {
        if (fprintf(args->out, "2\n") < 0) {
            err_c(STR("Failed to write to file."));
            return false;
        }
        ++cnt;
        ++start;
    }

    for (size_t i = start; cnt < args->end; i += 2) {
        if (es_is_prime(&pm, i)) {
            if (fprintf(args->out, "%zu\n", i) < 0) {
                err_c(STR("Failed to write to file."));
                return false;
            }
            ++cnt;
        }
    }

    es_delete(&pm);

    return true;
}

static bool nth_estimate(Args *args) {
    if (fprintf(args->out, "%zu\n", est_nth_prime(args->end)) < 0) {
        err_c(STR("Failed to write to file."));
        return false;
    }
    return true;
}
