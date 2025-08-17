#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <stddef.h>
#include <stdio.h>
#include <string.h>

/// String with length.
typedef struct {
    /// The string as C string.
    char *str;
    /// Length of the string.
    size_t len;
    size_t capacity;
} Str;

/// @brief Create borrowed string from string literal.
/// @param lit string literal.
/// @returns New borrowed string with static lifetime.
#define STR(lit) ((Str){ .str = (lit), .len = sizeof(lit) - 1, .capacity = 0 })

/// None string.
#define STR_NONE ((Str){ .str = nullptr, .len = 0, .capacity = 0 })

/// Empty string.
#define STR_EMPTY STR("")

/// @brief Create owned string with [`printf`] like formatting.
/// @param fmt Formatting string same as in [`printf`].
/// @param ... Parameters matching the format string `fmt`.
/// @returns New owned string created from the format. On error returns
/// [`str_none`] and `err` is populated.
Str str_fmt(const char *fmt, ...);

/// @brief Create owned string [`printf`] like formatting.
/// @param fmt Formatting string same as in [`printf`].
/// @param args Variadic argument list.
/// @returns New owned string created from the format. On error returns
/// [`str_none`] and `err` is populated.
Str str_vfmt(const char *fmt, va_list args);

/// Free data owned by the string and make it [`str_none`].
void str_delete(Str *s);

/// @brief Reserve memory so that `cap` bytes may be stored in the string
/// without reallocation.
///
/// This may make `s` owned.
/// @param s String where the memory will be reserved.
/// @param cap the target capacity in chars for the string.
bool str_reserve(Str *s, size_t cap);

/// @brief Remove `n` bytes from the end of `s`.
///
/// This may make `s` owned.
/// @param s String from which to pop.
/// @param n Number of bytes to pop.
/// @returns Number of bytes that were actually popped. `0` on error. This
/// means that error occured if `s->len != 0`, `n != 0` and this returns `0`.
///
/// On error `err` is populated and `s` is unmodified´.
size_t str_pop_n(Str *s, size_t n);

/// @brief Append `ext` given string to the string `s`.
/// @param s String which will be modified.
/// @param[in] ext String to append to `s`.
/// @returns `true` on success, otherwise `false` and `err` is popupated.
///
/// On error `s` is unmodified.
bool str_push(Str *s, const Str *ext);

/// @brief Borrow the given C string.
/// @param[in] s C string that will be borrowed.
/// @returns New borrowed string.
static inline Str str_borrow_c(const char *s) {
    return (Str){
        .str = (char *)s,
        .len = strlen(s),
        .capacity = 0,
    };
}

/// @brief Checks if `s` is owned string.
/// @param[in] s String to check.
/// @returns `true` if string is borrowed, otherwise `false`.
static inline bool str_is_owned(const Str *s) {
    return s->capacity;
}

/// @brief Checks if `s` is none.
/// @param[in] s String to check.
/// @returns `true` if `s` is none, otherwise `false`.
static inline bool str_is_none(const Str *s) {
    return !s->str;
}

/// @brief Check if `s` ends with `e`.
/// @param[in] s String which end will be checked.
/// @param[in] e The ending that will be checked.
/// @returns `true` if `s` ends with `e`, otherwise `false`.
static inline bool str_ends_with(const Str *s, const Str *e) {
    auto end_start = s->str + s->len - e->len;
    return e->len <= s->len &&
           (end_start == e->str ||
            memcmp(s->str + s->len - e->len, e->str, e->len) == 0);
}

/// @brief Check if the strings are equal.
/// @param[in] a First of the strings.
/// @param[in] b Second of the strings.
/// @returns `true` if the strings are equal, otherwise `false`.
static inline bool str_eq(const Str *a, const Str *b) {
    return a->len == b->len &&
           (a->str == b->str || memcmp(a->str, b->str, a->len) == 0);
}

/// @brief makes `s` owned string.
/// @param s String to make owned.
/// @returns `true` on success, otherwise `false` and `err` is populated.
///
/// On error `s` is unmodified.
static inline bool str_to_owned(Str *s) {
    return str_reserve(s, s->len);
}

/// @brief Check if `s` starts with `start`.
/// @param[in] s The string which start is checked.
/// @param[in] start String to check on start.
/// @returns `true` if `s` starts with `start`, otherwise `false`.
static inline bool str_starts_with(const Str *s, const Str *start) {
    return s->len >= start->len && memcmp(s->str, start->str, start->len) == 0;
}

#endif // STRING_H_INCLUDED