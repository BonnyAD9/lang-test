#include "err.h"

#include <__stdarg_va_list.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <threads.h>

#include "str.h"
#include "term.h"

thread_local static struct {
    Str msg;
    // Guard is here to avoid infinite recursion which may happen when error
    // occurs when raising error.
    bool guard;
} errs;

void err_c_code(Str msg, int code);

void err(Str msg) {
    if (str_is_none(&errs.msg)) {
        errs.msg = msg;
    }
}

void err_fmt(const char *fmt, ...) {
    if (errs.guard) {
        err(STR("Failed to generate error message."));
        return;
    }
    errs.guard = true;

    va_list args;
    va_start(args);
    err(str_vfmt(fmt, args));
    va_end(args);

    errs.guard = false;
}

void err_c(Str msg) {
    if (errs.guard) {
        err(STR("Failed to generate error message."));
        return;
    }
    errs.guard = true;
    err_c_code(msg, errno);
    errs.guard = false;
}

void err_c_fmt(const char *fmt, ...) {
    if (errs.guard) {
        err(STR("Failed to generate error message."));
    }
    errs.guard = true;

    auto code = errno;
    va_list args;
    va_start(args);
    err_c_code(str_vfmt(fmt, args), code);
    va_end(args);

    errs.guard = false;
}

void err_c_code(Str msg, int code) {
    if (!str_is_none(&errs.msg)) {
        return;
    }

    char buf[256];
    auto res = strerror_r(code, buf, sizeof(buf));
    auto err_msg = str_borrow_c(buf);

    if (msg.len == 0) {
        if (res != 0 || !str_to_owned(&err_msg)) {
            err(STR("Failed to generate error message."));
            return;
        }
        err(err_msg);
        return;
    }

    if (res != 0 || !str_reserve(&msg, msg.len + err_msg.len + 2)) {
        err(msg);
        return;
    }

    if (str_ends_with(&msg, &STR("."))) {
        str_pop_n(&msg, 1);
    }
    str_push(&msg, &STR(": "));
    str_push(&msg, &err_msg);

    err(msg);
}

Str err_pop() {
    auto res = errs.msg;
    errs.msg = STR_NONE;
    return res;
}

bool err_any() {
    return !str_is_none(&errs.msg);
}

void err_print() {
    if (!err_any()) {
        return;
    }

    auto msg = err_pop();
    fprintf(stderr, T_RED "error:" T_RESET " %s\n", msg.str);
    str_delete(&msg);
}
