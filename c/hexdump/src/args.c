#include "args.h"

#include "err.h"
#include "help.h"
#include "str.h"

bool args_short_opts(Args *args, char ***argv, const Str *arg);

#define ARGS_ERR ((Args){ .action = ACT_ERR })

#define NEXT_ARG(argv, dst, err)                                              \
    do {                                                                      \
        auto cur = *(argv);                                                   \
        if (!*++(argv)) {                                                     \
            err_fmt("Expected next argument after `%s`.", cur);               \
            return (err);                                                     \
        }                                                                     \
        (dst) = str_borrow_c(*(argv));                                        \
    } while (0)

Args args_parse(char **argv) {
    Args args = {
        .label = true,
    };

    while (*++argv) {
        auto arg = str_borrow_c(*argv);
        if (*arg.c == '-' && arg.c[1] != '-') {
            if (!args_short_opts(&args, &argv, &arg)) {
                return ARGS_ERR;
            }
        } else if (STR_EQ(&arg, "--help")) {
            help();
            args.action = ACT_HELP;
        } else if (STR_EQ(&arg, "--file")) {
            NEXT_ARG(argv, args.file, ARGS_ERR);
        } else if (STR_EQ(&arg, "--no-label")) {
            args.label = false;
        } else if (STR_EQ(&arg, "--label")) {
            args.label = true;
        } else {
            args.file = arg;
        }
    }

    if (args.action != ACT_HELP && str_is_none(&args.file)) {
        ERR("Missing file to dump.");
        return ARGS_ERR;
    }

    return args;
}

bool args_short_opts(Args *args, char ***argv, const Str *arg) {
    auto c = arg->c;
    while (*++c) {
        switch (*c) {
        case '?':
        case 'h':
            help();
            args->action = ACT_HELP;
            break;
        case 'f':
            NEXT_ARG(*argv, args->file, false);
            break;
        case 'L':
            args->label = false;
            break;
        case 'l':
            args->label = true;
            break;
        default:
            err_fmt("Unknown short option `%c`", *c);
            return false;
        }
    }
    return true;
}