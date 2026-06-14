#ifndef TEST_H
#define TEST_H

/* Minimal zero-dependency test harness. Each CHECK updates global counters
   defined in test_main.c and prints a one-line result. */

#include <stdio.h>
#include <string.h>

extern int g_tests_run;
extern int g_tests_failed;

#define SUITE(name) printf("\n[%s]\n", name)

#define CHECK(cond, desc)                                                  \
  do {                                                                     \
    g_tests_run++;                                                         \
    if (cond) {                                                            \
      printf("  ok   %s\n", desc);                                         \
    } else {                                                               \
      g_tests_failed++;                                                    \
      printf("  FAIL %s  (%s:%d)\n", desc, __FILE__, __LINE__);            \
    }                                                                      \
  } while (0)

#define CHECK_INT_EQ(actual, expected, desc)                               \
  do {                                                                     \
    long _a = (long)(actual);                                              \
    long _e = (long)(expected);                                            \
    g_tests_run++;                                                         \
    if (_a == _e) {                                                        \
      printf("  ok   %s\n", desc);                                         \
    } else {                                                               \
      g_tests_failed++;                                                    \
      printf("  FAIL %s: got %ld, want %ld  (%s:%d)\n", desc, _a, _e,      \
             __FILE__, __LINE__);                                          \
    }                                                                      \
  } while (0)

#define CHECK_STR_EQ(actual, expected, desc)                               \
  do {                                                                     \
    const char *_a = (actual);                                             \
    const char *_e = (expected);                                           \
    g_tests_run++;                                                         \
    if (_a && _e && strcmp(_a, _e) == 0) {                                 \
      printf("  ok   %s\n", desc);                                         \
    } else {                                                               \
      g_tests_failed++;                                                    \
      printf("  FAIL %s: got \"%s\", want \"%s\"  (%s:%d)\n", desc,        \
             _a ? _a : "(null)", _e ? _e : "(null)", __FILE__, __LINE__);  \
    }                                                                      \
  } while (0)

#endif /* TEST_H */
