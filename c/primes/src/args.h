#ifndef ARGS_H_INCLUDED
#define ARGS_H_INCLUDED

#include <stddef.h>

/// What should be done. Type of operation of the program.
typedef enum {
    TYPE_ERROR,
    TYPE_HELP,
    TYPE_DEFAULT,
    TYPE_COUNT,
    TYPE_NTH,
} Type;

typedef enum {
    MODE_SINGLE,
    MODE_RANGED,
    MODE_ESTIMATE,
} Mode;

/// Parsed command line arguments.
typedef struct {
    size_t start;
    size_t end;
    Type type;
    Mode mode;
} Args;

/// @brief Parse the command line arguments.
/// @param[in] args Command line arguments (`argv`).
/// @returns Parsed arguments. On error mode is [`MODE_ERROR`] and error stack
/// has set the error.
Args arg_parse(const char *const *args);

#endif // ARGS_H_INCLUDED