#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED

#include "str.h"

#define ERR_C(lit) err_c(STR(lit))

#define ERR(lit) err(STR(lit))

void err(Str s);

void err_c(Str msg);

void err_vfmt(const char *fmt, va_list args);

void err_fmt(const char *fmt, ...);

void err_c_fmt(const char *fmt, ...);

bool err_any();

Str err_pop();

#endif // ERR_H_INCLUDED