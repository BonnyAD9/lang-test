#include "err.h"
#include <errno.h>
#include <stdio.h>
#include <threads.h>
#include "str.h"
#include "term.h"

thread_local static struct {
    Str msg;
} errs;

void err(Str msg) {
    if (str_is_none(&errs.msg)) {
        errs.msg = msg;
    }
}

void err_c(Str msg) {
    if (!str_is_none(&errs.msg)) {
        return;
    }
    
    thread_local static bool guard = false;
    auto code = errno;
    if (guard) {
        err(msg);
        return;
    }
    guard = true;
    
    auto err_msg = str_borrow_c(strerror(code));
    
    if (msg.len == 0) {
        if (!str_to_owned(&err_msg)) {
            err(STR("Failed to generate error message."));
            guard = false;
            return;
        }
        err(err_msg);
        guard = false;
        return;
    }
    
    if (!str_reserve(&msg, msg.len + err_msg.len + 2)) {
        err(msg);
        guard = false;
        return;
    }
    
    if (str_ends_with(&msg, &STR("."))) {
        str_pop_n(&msg, 1);
    }
    str_push(&msg, &STR(": "));
    str_push(&msg, &err_msg);
    
    err(msg);
    guard = false;
}

Str err_pop() {
    auto res = errs.msg;
    errs.msg = str_none();
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
