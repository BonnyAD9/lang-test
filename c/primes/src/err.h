#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED

#include "str.h"

/// @brief Raise error with the given message.
/// @param msg Error message.
void err(Str msg);

/// @brief Raise errorr with the given message. Also fetch message from C error
/// stack.
/// @param msg Error message. Will be appended with C error message.
void err_c(Str msg);

/// @brief Pop the error. The error will be cleared.
/// @returns Error message. [`str_none`] if there is no error.
Str err_pop();

/// @brief Checks if there is any error.
/// @returns `true` if there is any error, otherwise `false`.
bool err_any();

/// Prints the current error (if any) to stderr.
void err_print();

#endif // ERR_H_INCLUDED