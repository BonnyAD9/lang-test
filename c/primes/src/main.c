#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "err.h"
#include "help.h"
#include "primap.h"

static void start(Args *args);
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
    case MODE_COUNT:
        count(args);
        break;
    case MODE_NTH:
        nth(args);
        break;
    }
}

static bool count(Args *args) {
    auto pm = pm_new();
    auto res = pm_is_prime(&pm, args->num);
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

static bool nth(Args *args) {
    auto pm = pm_new();
    auto res = pm_nth(&pm, args->num);
    pm_delete(&pm);
    if (res == 0) {
        return false;
    }

    printf("%zu\n", res);
    return true;
}
