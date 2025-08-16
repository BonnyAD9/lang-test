#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/// @brief Get the smaller number of the two. If they are the same, get the
/// first.
///
/// This will evaluate the smaller parameter twice.
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

/// @brief Get the larger number of the two. If they are the same, get the
/// first.
///
/// This will evaluate the larger parameter twice.
#define MAX(a, b) ((a) >= (b) ? (a) : (b))

#endif // UTILS_H_INCLUDED