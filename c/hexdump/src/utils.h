#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "stdio.h"
#include "stdlib.h"

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

#define MIN(a, b) ((a) <= (b) ? (a) : (b))

#define FIRST(a, ...) a
#define TAIL(a, ...) __VA_ARGS__
#define COMMA_ARGS(...) __VA_OPT__(, __VA_ARGS__)

#define TODO(...)                                                             \
    do {                                                                      \
        fprintf(                                                              \
            stderr,                                                           \
            "TODO " __VA_OPT__(FIRST(__VA_ARGS__)) "\n" COMMA_ARGS(           \
                __VA_OPT__(TAIL(__VA_ARGS__))                                 \
            )                                                                 \
        );                                                                    \
        exit(EXIT_FAILURE);                                                   \
    } while (0)

#endif // UTILS_H_INCLUDED