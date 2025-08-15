#include "str.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "utils.h"

Str str_fmt(const char *fmt, ...) {
    va_list args;
    va_start(args);
    auto len = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);
    if (len < 0) {
        err_c(STR("Failed to determine fmt string length."));
        return str_none();
    }

    char *buf = malloc(len + 1);
    if (!buf) {
        err_c(STR("Failed to allocate data for string."));
        return str_none();
    }

    va_start(args);
    len = vsnprintf(buf, len + 1, fmt, args);
    va_end(args);
    if (len < 0) {
        free(buf);
        err_c(STR("Failed to write fmt string."));
        return str_none();
    }

    return (Str){
        .capacity = len,
        .str = buf,
        .len = len,
    };
}

void str_delete(Str *s) {
    if (str_is_owned(s)) {
        free(s->str);
    }
    s->str = "";
    s->len = 0;
    s->capacity = 0;
}

bool str_reserve(Str *s, size_t cap) {
    auto capacity = MAX(s->len, MAX(s->capacity, cap));
    if (s->capacity == capacity) {
        return true;
    }

    char *buf;
    if (str_is_owned(s)) {
        buf = realloc(s->str, capacity + 1);
        if (!buf) {
            err_c(STR("Failed to reallocate data to resize string."));
            return false;
        }
    } else {
        buf = malloc(capacity + 1);
        if (!buf) {
            err_c(STR("Failed to allocate data for owned string."));
            return false;
        }
        memcpy(buf, s->str, s->len + 1);
    }

    s->str = buf;
    s->capacity = capacity;

    return true;
}

size_t str_pop_n(Str *s, size_t n) {
    if (!str_to_owned(s)) {
        return 0;
    }
    auto cnt = MIN(n, s->len);
    s->len -= cnt;
    s->str[s->len] = 0;
    return cnt;
}

bool str_push(Str *s, const Str *ext) {
    if (!str_reserve(s, s->len + ext->len)) {
        return false;
    }

    memmove(s->str + s->len, ext->str, ext->len + 1);
    s->len += ext->len;
    return true;
}
