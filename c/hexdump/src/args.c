#include "args.h"

#include "err.h"
#include "help.h"
#include "str.h"

#define ARGS_ERR ((Args){ .action = ACT_ERR })

#define NEXT_ARG(argv, dst)                                                   \
    do {                                                                      \
        auto cur = *(argv);                                                   \
        if (!*++(argv)) {                                                     \
            err_fmt("Expected next argument after `%s`.", cur);               \
            return ARGS_ERR;                                                  \
        }                                                                     \
        (dst) = str_borrow_c(*(argv));                                        \
    } while (0)

Args args_parse(char **argv) {
    Args args = { 0 };

    while (*++argv) {
        auto arg = str_borrow_c(*argv);
        if (STR_EQ(&arg, "-h") || STR_EQ(&arg, "-?") ||
            STR_EQ(&arg, "--help")) {
            help();
            args.action = ACT_HELP;
        } else if (STR_EQ(&arg, "-f") || STR_EQ(&arg, "--file")) {
            NEXT_ARG(argv, args.file);
        } else if (*arg.c == '-') {
            err_fmt("Unknown argument `%s`.", arg.c);
            return ARGS_ERR;
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