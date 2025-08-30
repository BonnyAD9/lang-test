#include "err.h"

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <threads.h>

#include "str.h"

static void err_c_code(Str msg, int code);

static thread_local Str err_msg = STR_NONE;
static thread_local bool err_guard = false;

void err(Str s) {
    if (!err_any()) {
        err_msg = s;
    } else {
        str_free(&s);
    }
}

void err_vfmt(const char *fmt, va_list args) {
    if (err_any()) {
        return;
    }

    if (err_guard) {
        err(STR("Failed to generate error message."));
        return;
    }
    err_guard = true;

    auto msg = str_vfmt(fmt, args);
    if (str_is_none(&msg)) {
        err(STR("Failed to generate error message."));
    } else {
        err(msg);
    }

    err_guard = false;
}

void err_fmt(const char *fmt, ...) {
    va_list args;
    va_start(args);
    err_vfmt(fmt, args);
    va_end(args);
}

void err_c_fmt(const char *fmt, ...) {
    int code = errno;
    if (err_any()) {
        return;
    }

    if (err_guard) {
        err(STR("Failed to generate error message."));
        return;
    }
    err_guard = true;

    va_list args;
    va_start(args);
    auto msg = str_vfmt(fmt, args);
    va_end(args);
    err_c_code(msg, code);

    err_guard = false;
}

bool err_any() {
    return !str_is_none(&err_msg);
}

Str err_pop() {
    auto res = err_msg;
    err_msg = STR_NONE;
    return res;
}

void err_c(Str msg) {
    int code = errno;
    if (err_any()) {
        return;
    }

    if (err_guard) {
        ERR("Failed to generate error message.");
    }

    err_c_code(msg, code);
}

static void err_c_code(Str msg, int code) {
    char buf[256];
    if (strerror_r(code, buf, sizeof(buf)) != 0) {
        buf[0] = 0;
    }

    if (str_is_none(&msg) || msg.len == 0) {
        str_free(&msg);
        msg = STR_EMPTY;
    }

    if (str_ends_with(&msg, &STR("."))) {
        if (str_pop_n(&msg, 1) != 1) {
            goto on_err;
        }
        if (!str_append(&msg, &STR(": "))) {
            goto on_err;
        }
    }

    auto c_msg = str_borrow_c(buf);

    if (!str_append(&msg, &c_msg)) {
        goto on_err;
    }

    err(msg);
    return;
on_err:
    if (msg.len == 0) {
        str_free(&msg);
        err(STR("Failed to generate error message."));
    } else {
        err(msg);
    }
}