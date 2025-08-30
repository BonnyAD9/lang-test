#include "str.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "utils.h"

Str str_vfmt(const char *fmt, va_list args) {
    va_list args2;
    va_copy(args2, args);
    auto cnt = vsnprintf(nullptr, 0, fmt, args2);
    va_end(args2);
    if (cnt < 0) {
        ERR_C("Failed to format string.");
        return STR_NONE;
    }

    Str res = STR_EMPTY;
    if (!str_reserve_exact(&res, cnt)) {
        return STR_NONE;
    }

    cnt = vsnprintf(res.c, res.capacity + 1, fmt, args);
    if (cnt < 0) {
        str_free(&res);
        ERR_C("Failed to format string");
        return STR_NONE;
    }

    res.len = cnt;
    return res;
}

bool str_reserve_exact(Str *str, size_t size) {
    if (str_is_none(str) && size == 0) {
        *str = STR_EMPTY;
    }
    if (str->capacity >= size) {
        return true;
    }

    size = MAX(size, str->len);
    char *new_c =
        str_is_owned(str) ? realloc(str->c, size + 1) : malloc(size + 1);
    if (!new_c) {
        ERR_C("Failed to allocate data for string.");
        return false;
    }

    if (!str_is_owned(str)) {
        if (str->len == 0) {
            *new_c = 0;
        } else {
            memcpy(new_c, str->c, str->len + 1);
        }
    }

    str->c = new_c;
    str->capacity = size;
    return true;
}

bool str_append(Str *str, const Str *s) {
    if (s->len == 0) {
        return true;
    }

    if (!str_reserve(str, str->len + s->len)) {
        return false;
    }

    memmove(str->c + str->len, s->c, s->len + 1);
    return true;
}

void str_free(Str *str) {
    if (str_is_owned(str)) {
        free(str->c);
    }
    *str = STR_NONE;
}

bool str_ends_with(const Str *str, const Str *end) {
    if (str->len < end->len) {
        return false;
    }
    auto pos = str->c + str->len - end->len;
    return pos == end->c || memcmp(pos, end->c, end->len) == 0;
}

size_t str_pop_n(Str *str, size_t n) {
    n = MIN(str->len, n);
    if (n == 0) {
        return 0;
    }

    size_t new_len = str->len - n;

    if (!str_is_owned(str)) {
        char *new_c = malloc(new_len + 1);
        if (!new_c) {
            ERR_C("Failed to allocate string.");
            return 0;
        }
        memcpy(new_c, str->c, new_len);
        str->c = new_c;
    }

    str->c[new_len] = 0;
    str->len = new_len;
    return n;
}
