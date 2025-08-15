#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <stddef.h>
#include <string.h>

typedef struct {
    char *str;
    size_t len;
    size_t capacity;
} Str;

#define STR(lit) ((Str){ .str = (lit), .len = sizeof(lit) - 1, .capacity = 0 })

Str str_fmt(const char *fmt, ...);

void str_delete(Str *s);

bool str_reserve(Str *s, size_t cap);

size_t str_pop_n(Str *s, size_t n);

bool str_push(Str *s, const Str *ext);

static inline Str str_empty() {
    return (Str){
        .str = "",
        .len = 0,
        .capacity = 0,
    };
}

static inline Str str_none() {
    return (Str){
        .str = nullptr,
        .len = 0,
        .capacity = 0,
    };
}

static inline Str str_borrow_c(const char *s) {
    return (Str){
        .str = (char *)s,
        .len = strlen(s),
        .capacity = 0,
    };
}

static inline bool str_is_owned(const Str *s) {
    return s->capacity;
}

static inline bool str_is_none(const Str *s) {
    return !s->str;
}

static inline bool str_ends_with(const Str *s, const Str *e) {
    return e->len <= s->len &&
           memcmp(s->str + s->len - e->len, e->str, e->len) == 0;
}

static inline bool str_eq(const Str *a, const Str *b) {
    return a->len == b->len && memcmp(a->str, b->str, a->len) == 0;
}

static inline bool str_to_owned(Str *s) {
    return str_reserve(s, s->len);
}

static inline bool str_starts_with(const Str *s, const Str *start) {
    return s->len >= start->len && memcmp(s->str, start->str, start->len) == 0;
}

#endif // STRING_H_INCLUDED