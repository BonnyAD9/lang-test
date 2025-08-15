#include "args.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "str.h"

static size_t parse_size(const Str *s);
static Args arg_err();

Args arg_parse(char **args) {
    Args res = {
        .mode = MODE_COUNT,
        .num = 0,
    };

    bool num_set = false;

    while (*++args) {
        auto arg = str_borrow_c(*args);

        if (str_eq(&arg, &STR("-h")) || str_eq(&arg, &STR("-?")) ||
            str_eq(&arg, &STR("--help"))) {
            res.mode = MODE_HELP;
        } else if (str_eq(&arg, &STR("-n")) || str_eq(&arg, &STR("--nth"))) {
            res.mode = MODE_NTH;
        } else {
            res.num = parse_size(&arg);
            if (err_any()) {
                return arg_err();
            }
            num_set = true;
        }
    }

    if (!num_set && res.mode != MODE_HELP) {
        err(STR("Missing number argument."));
        return arg_err();
    }

    return res;
}

static size_t parse_size(const Str *s) {
    if (str_starts_with(s, &STR("-"))) {
        err(str_fmt("Expected `%s` to be positive number.", s->str));
        return 0;
    }

    char *e;
    auto res = strtoull(s->str, &e, 0);
    if (*e && (res == 0 || res == ULLONG_MAX)) {
        if (res == 0 || res == ULLONG_MAX) {
            err_c(
                str_fmt("Failed to parse `%s` to unsigned integer.", s->str)
            );
        } else {
            err(str_fmt("Failed to parse `%s` to unsigned integer.", s->str));
        }
        return 0;
    }

    return res;
}

static Args arg_err() {
    return (Args){ .mode = MODE_ERROR };
}