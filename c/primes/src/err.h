#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED

#include "str.h"

void err(Str msg);

void err_c(Str msg);

Str err_pop();

bool err_any();

void err_print();

#endif // ERR_H_INCLUDED