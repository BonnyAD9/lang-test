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

/// Noop, clang-tidy thinks that this is noreturn function.
static inline bool clang_noreturn() __attribute__((analyzer_noreturn)) {
    return false;
}

// This assertion does nothing on runtime, but it is evaluated by clang-tidy.
// It is used to silence false positives.
#define CLANG_ASSERT(cond) ((cond) ? true : clang_noreturn())

#endif // UTILS_H_INCLUDED