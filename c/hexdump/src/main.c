#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "err.h"

void start(Args *args);
bool hexdump_file(const Str *path);

int main(int, char **argv) {
    auto args = args_parse(argv);
    start(&args);
    if (!err_any()) {
        return EXIT_SUCCESS;
    }
    auto msg = err_pop();
    fprintf(stderr, "error: %s", msg.c);
    str_free(&msg);
    return EXIT_FAILURE;
}

void start(Args *args) {
    switch (args->action) {
    case ACT_DUMP:
        hexdump_file(&args->file);
        return;
    case ACT_HELP:
    case ACT_ERR:
        return;
    }
}

bool hexdump_file(const Str *path) {
    auto file = fopen(path->c, "r");
    if (!file) {
        err_c_fmt("Failed to open file `%s`.", path->c);
        return false;
    }

    char buf[16];

    while (!feof(file) && !ferror(file)) {
        size_t cnt = fread(buf, 1, 16, file);
        for (size_t i = 0; i < cnt - 1; ++i) {
            printf("%02hhX ", buf[i]);
        }
        printf("%02hhX\n", buf[cnt - 1]);
    }

    if (ferror(file)) {
        err_c_fmt("Failed reading from file `%s`.", path->c);
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}
