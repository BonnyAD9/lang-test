#ifndef ARGS_H_INCLUDED
#define ARGS_H_INCLUDED

#include "str.h"

typedef enum {
    ACT_DUMP,
    ACT_HELP,
    ACT_ERR,
} Action;

typedef struct {
    Action action;
    Str file;
    bool label;
    bool ascii;
} Args;

Args args_parse(char **argv);

#endif // ARGS_H_INCLUDED