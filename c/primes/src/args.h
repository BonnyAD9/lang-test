#ifndef ARGS_H_INCLUDED
#define ARGS_H_INCLUDED

#include <stddef.h>

typedef enum {
    MODE_ERROR,
    MODE_HELP,
    MODE_COUNT,
    MODE_NTH,
} Mode;

typedef struct {
    Mode mode;
    size_t num;
} Args;

Args arg_parse(char **args);

void arg_perror(Args *args);

#endif // ARGS_H_INCLUDED