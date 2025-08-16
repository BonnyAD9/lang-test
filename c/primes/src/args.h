#ifndef ARGS_H_INCLUDED
#define ARGS_H_INCLUDED

#include <stddef.h>

/// What should be done. Mode of operation of the program.
typedef enum {
    MODE_ERROR,
    MODE_HELP,
    MODE_DEFAULT,
    MODE_COUNT,
    MODE_NTH,
} Mode;

/// Parsed command line arguments.
typedef struct {
    size_t start;
    size_t end;
    Mode mode;
    bool ranged;
} Args;

/// @brief Parse the command line arguments.
/// @param[in] args Command line arguments (`argv`).
/// @returns Parsed arguments. On error mode is [`MODE_ERROR`] and error stack
/// has set the error.
Args arg_parse(const char *const *args);

#endif // ARGS_H_INCLUDED