#ifndef ARGS_H_INCLUDED
#define ARGS_H_INCLUDED

#include <stddef.h>

typedef enum {
    MODE_ERROR,
    MODE_HELP,
    MODE_DEFAULT,
    MODE_COUNT,
    MODE_NTH,
} Mode;

typedef struct {
    size_t start;
    size_t end;
    Mode mode;
    bool ranged;
} Args;

Args arg_parse(char **args);

void arg_perror(Args *args);

#endif // ARGS_H_INCLUDED