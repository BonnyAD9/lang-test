#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "err.h"
#include "utils.h"

static void start(Args *args);
static bool hexdump_file(const Args *args);
static void print_hex(const char *buf, size_t len);
static void print_ascii(char *buf, size_t len);
static size_t file_len(FILE *f);

static constexpr size_t LINE_SIZE = 16;

int main(int, char **argv) {
    auto args = args_parse(argv);
    start(&args);
    if (!err_any()) {
        return EXIT_SUCCESS;
    }
    auto msg = err_pop();
    fprintf(stderr, "error: %s\n", msg.c);
    str_free(&msg);
    return EXIT_FAILURE;
}

static void start(Args *args) {
    switch (args->action) {
    case ACT_DUMP:
        hexdump_file(args);
        return;
    case ACT_HELP:
    case ACT_ERR:
        return;
    }
}

static bool hexdump_file(const Args *args) {
    auto file = fopen(args->file.c, "rb");
    if (!file) {
        err_c_fmt("Failed to open file `%s`.", args->file.c);
        return false;
    }

    auto flen = file_len(file);
    auto label_len = args->label ? (stdc_bit_width(flen) + 3) / 4 : 0;
    const char label[] = "%%0%uzX   ";
    char label_fmt[sizeof(label)] = { 0 };
    if (label_len != 0) {
        sprintf(label_fmt, label, label_len);
    }

    char buf[LINE_SIZE + 1];
    for (size_t pos = 0; !feof(file) && !ferror(file); pos += LINE_SIZE) {
        printf(label_fmt, pos);
        size_t cnt = fread(buf, 1, LINE_SIZE, file);
        buf[cnt] = 0;
        print_hex(buf, cnt);
        if (args->ascii) {
            printf("   ");
            print_ascii(buf, cnt);
        }
        printf("\n");
    }

    if (ferror(file)) {
        err_c_fmt("Failed reading from file `%s`.", args->file.c);
        fclose(file);
        return false;
    }

    if (label_len != 0 && flen % 4 != 0) {
        printf(label_fmt, flen);
        printf("\n");
    }

    fclose(file);
    return true;
}

static void print_hex(const char *buf, size_t len) {
    auto flim = MIN(len, LINE_SIZE / 2);
    size_t i = 0;
    for (; i < flim; ++i) {
        printf("%02hhX ", buf[i]);
    }
    for (; i < len; ++i) {
        printf(" %02hhX", buf[i]);
    }
    for (; i < LINE_SIZE; ++i) {
        printf("   ");
    }
}

static void print_ascii(char *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (!isprint((unsigned char)buf[i])) {
            buf[i] = '.';
        }
    }
    printf("|%s|", buf);
}

static size_t file_len(FILE *f) {
    auto pos0 = ftell(f);
    if (pos0 < -1) {
        goto on_err;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        goto on_err;
    }

    auto pos = ftell(f);
    if (pos == -1) {
        ERR_C("Failed to get file length.");
    }

    fseek(f, pos0, SEEK_SET);
    return pos;
on_err:
    ERR_C("Failed to get file length");
    return SIZE_MAX;
}
