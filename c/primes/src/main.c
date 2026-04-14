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
static bool default_factors(Args *args);
static bool default_aggregate_factors(Args *args);
static bool count(Args *args);
static bool count_single(Args *args);
static bool count_ranged(Args *args);
static bool count_estimate(Args *args);
static bool count_factors(Args *args);
static bool count_aggregate_factors(Args *args);
static bool nth(Args *args);
static bool nth_single(Args *args);
static bool nth_ranged(Args *args);
static bool nth_estimate(Args *args);
static bool nth_factor(Args *args);
static bool nth_aggregate_factor(Args *args);

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
    case MODE_FACTOR:
        return default_factors(args);
    case MODE_AGGREGATE_FACTOR:
        return default_aggregate_factors(args);
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

static bool default_factors(Args *args) {
    auto fs = factors_init(args->end);

    size_t res = 0;
    for (auto n = next_factor(&fs); n != 0; n = next_factor(&fs)) {
        fprintf(args->out, "%zu\n", n);
        ++res;
    }

    fprintf(args->out, "total of %zu factors\n", res);

    return true;
}

static bool default_aggregate_factors(Args *args) {
    auto fs = factors_init(args->end);

    size_t cnt = 0;
    size_t dcnt = 0;
    auto n = next_factor(&fs);
    auto ln = n;
    size_t nc = 0;
    for (; n != 0; n = next_factor(&fs)) {
        ++cnt;
        if (n != ln) {
            fprintf(args->out, "%zu^%zu\n", ln, nc);
            ++dcnt;
            ln = n;
            nc = 1;
        } else {
            ++nc;
        }
    }

    if (cnt != 0) {
        fprintf(args->out, "%zu^%zu\n", ln, nc);
        ++dcnt;
    }

    fprintf(
        args->out, "total of %zu factors in %zu distinct primes\n", cnt, dcnt
    );

    return true;
}

static bool count(Args *args) {
    switch (args->mode) {
    case MODE_SINGLE:
        return count_single(args);
    case MODE_RANGED:
        return count_ranged(args);
    case MODE_ESTIMATE:
        return count_estimate(args);
    case MODE_FACTOR:
        return count_factors(args);
    case MODE_AGGREGATE_FACTOR:
        return count_aggregate_factors(args);
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

static bool count_factors(Args *args) {
    auto fs = factors_init(args->end);

    size_t res = 0;
    for (auto n = next_factor(&fs); n != 0; n = next_factor(&fs)) {
        ++res;
    }

    fprintf(args->out, "%zu factors\n", res);

    return true;
}

static bool count_aggregate_factors(Args *args) {
    auto fs = factors_init(args->end);

    size_t cnt = 0;
    size_t dcnt = 0;
    auto n = next_factor(&fs);
    auto ln = n;
    for (; n != 0; n = next_factor(&fs)) {
        ++cnt;
        if (n != ln) {
            ++dcnt;
            ln = n;
        }
    }

    if (cnt != 0) {
        ++dcnt;
    }

    fprintf(args->out, "%zu distinct factors\n", dcnt);

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
    case MODE_FACTOR:
        return nth_factor(args);
    case MODE_AGGREGATE_FACTOR:
        return nth_aggregate_factor(args);
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

static bool nth_factor(Args *) {
    err(STR("Nth factor is not valid combination."));
    return false;
}

static bool nth_aggregate_factor(Args *) {
    err(STR("Nth aggregate factor is not valid combination."));
    return false;
}
