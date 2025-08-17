#include "args.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "str.h"

static size_t parse_size(const Str *s);
static Args arg_err();

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
Args arg_parse(const char *const *args) {
    Args res = {
        .start = 0,
        .end = 0,
        .type = TYPE_DEFAULT,
        .mode = MODE_SINGLE,
    };

    bool num_set = false;

    while (*++args) {
        auto arg = str_borrow_c(*args);

        if (str_eq(&arg, &STR("-h")) || str_eq(&arg, &STR("-?")) ||
            str_eq(&arg, &STR("--help"))) {
            res.type = TYPE_HELP;
        } else if (str_eq(&arg, &STR("-n")) || str_eq(&arg, &STR("--nth"))) {
            res.type = TYPE_NTH;
        } else if (str_eq(&arg, &STR("-c")) || str_eq(&arg, &STR("--count"))) {
            res.type = TYPE_COUNT;
        } else if (str_eq(&arg, &STR("-e")) ||
                   str_eq(&arg, &STR("--estimate"))) {
            res.mode = MODE_ESTIMATE;
        } else if (str_eq(&arg, &STR("-r")) || str_eq(&arg, &STR("--range"))) {
            res.mode = MODE_RANGED;
        } else if (str_eq(&arg, &STR("-s")) || str_eq(&arg, &STR("--from"))) {
            res.mode = MODE_RANGED;
            if (!*++args) {
                err_fmt("Expected integer after `%s`.", arg.str);
                return arg_err();
            }
            auto next = str_borrow_c(*args);
            res.start = parse_size(&next);
            if (err_any()) {
                return arg_err();
            }
        } else {
            res.end = parse_size(&arg);
            if (err_any()) {
                return arg_err();
            }
            num_set = true;
        }
    }

    if (!num_set && res.type != TYPE_HELP) {
        err(STR("Missing number argument."));
        return arg_err();
    }

    if (res.start > res.end) {
        err(STR("Expected start of range to be smaller than the end."));
        return arg_err();
    }

    return res;
}

static size_t parse_size(const Str *s) {
    if (str_starts_with(s, &STR("-"))) {
        err_fmt("Expected `%s` to be positive number.", s->str);
        return 0;
    }

    char *e;
    auto res = strtoull(s->str, &e, 0);
    if (*e && (res == 0 || res == ULLONG_MAX)) {
        if (res == 0 || res == ULLONG_MAX) {
            err_c_fmt("Failed to parse `%s` to unsigned integer.", s->str);
        } else {
            err_fmt("Failed to parse `%s` to unsigned integer.", s->str);
        }
        return 0;
    }

    return res;
}

static Args arg_err() {
    return (Args){ .type = TYPE_ERROR };
}