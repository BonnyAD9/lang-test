#include <assert.h>
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
    auto pm = pm_new();
    if (!args->ranged) {
        auto res = pm_is_prime(&pm, args->end);
        pm_delete(&pm);
        switch (res) {
        case -1:
            return false;
        case 0:
            printf("%zu is not prime\n", args->end);
            break;
        case 1:
            printf("%zu is prime\n", args->end);
            break;
        default:
            assert(false);
            break;
        }

        return true;
    }

    if (args->start <= 2 && args->end >= 2) {
        printf("2\n");
    }

    for (size_t i = args->start | 1; i < args->end; i += 2) {
        switch (pm_is_prime(&pm, i)) {
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
    auto pm = pm_new();
    if (!args->ranged) {
        auto res = pm_is_prime(&pm, args->end);
        pm_delete(&pm);
        switch (res) {
        case -1:
            return false;
        case 0:
            printf("0\n");
            break;
        case 1:
            printf("1\n");
            break;
        default:
            assert(false);
            break;
        }

        return true;
    }

    if (args->end == args->start) {
        printf("0\n");
        pm_delete(&pm);
        return true;
    }

    if (pm_is_prime(&pm, args->end - 1) == -1) {
        pm_delete(&pm);
        return false;
    }

    size_t cnt = 0;

    for (size_t i = args->start | 1; i < args->end; i += 2) {
        cnt += pm_is_prime(&pm, i);
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
