#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <stdarg.h>
#include <stdbit.h>
#include <stddef.h>
#include <string.h>

typedef struct {
    char *c;
    size_t len;
    size_t capacity;
} Str;

#define STR(lit) ((Str){ .c = (lit), .len = sizeof(lit) - 1, .capacity = 0 })

#define STR_NONE ((Str){ 0 })

#define STR_EMPTY STR("")

#define STR_EQ(str, lit) str_eq(str, &STR(lit))

Str str_vfmt(const char *fmt, va_list args);

bool str_reserve_exact(Str *str, size_t size);

bool str_append(Str *str, const Str *s);

void str_free(Str *str);

bool str_ends_with(const Str *str, const Str *end);

size_t str_pop_n(Str *str, size_t n);

static inline bool str_is_owned(const Str *str) {
    return str->capacity;
}

static inline bool str_is_none(const Str *str) {
    return !str->c;
}

static inline Str str_borrow_c(const char *c_str) {
    return (Str){
        .c = (char *)c_str,
        .len = strlen(c_str),
    };
}

static inline bool str_reserve(Str *str, size_t size) {
    return str_reserve_exact(str, stdc_bit_ceil(size));
}

static inline bool str_eq(const Str *a, const Str *b) {
    return a->c == b->c ||
           (a->len == b->len && memcmp(a->c, b->c, a->len) == 0);
}

#endif // STR_H_INCLUDED