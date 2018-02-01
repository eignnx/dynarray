#ifndef MACROASSERT_H
#define MACROASSERT_H

#include <stdio.h>

#define assert_pred(lhs, rhs, pred) do { \
    if (lhs pred rhs) { \
        fprintf(stderr, "[Assertion error: %s:%d] " #lhs " " #pred " " #rhs "\n", __FILE__, __LINE__); \
        exit(-1); \
    } \
} while(0)

#define assert_eq(lhs, rhs) assert_pred(lhs, rhs, !=)
#define assert_neq(lhs, rhs) assert_pred(lhs, rhs, ==)

#define assert_gt(lhs, rhs) assert_pred(lhs, rhs, <=)
#define assert_lt(lhs, rhs) assert_pred(lhs, rhs, >=)

#define assert_gte(lhs, rhs) assert_pred(lhs, rhs, <)
#define assert_lte(lhs, rhs) assert_pred(lhs, rhs, >)

#endif // MACROASSERT_H
