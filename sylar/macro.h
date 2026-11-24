#ifndef __SYLAR_MACRO_H__
#define __SYLAR_MACRO_H__

#include <cstring>
#include <cassert>

#include "util.h"

#if defined __GNUC__ || defined __llvm__
#   define SYLAR_LICKLY(x)          __builtin_expect(!!(x), 1)
#   define SYLAR_UNLICKLY(x)        __builtin_expect(!!(x), 0)
#else
#   define SYLAR_LICKLY(x)          (x)
#   define SYLAR_UNLICKLY(x)        (x)
#endif

// Assert作用：当条件为假时，程序终止
#define SYLAR_ASSERT(x) \
    if (SYLAR_LICKLY(!(x))) { \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << sylar::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#define SYLAR_ASSERT2(x, w) \
    if (SYLAR_LICKLY(!(x))) { \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << sylar::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif