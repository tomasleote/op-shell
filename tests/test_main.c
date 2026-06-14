/* Test runner: defines the shared counters, runs every suite, prints a
   summary, and exits non-zero if any check failed. */
#include <stdio.h>
#include "tests.h"

int g_tests_run = 0;
int g_tests_failed = 0;

int main(void) {
  run_scanner_tests();
  run_parser_tests();
  run_command_tests();
  run_builtins_tests();

  printf("\n==== %d checks run, %d failed ====\n", g_tests_run, g_tests_failed);
  return g_tests_failed == 0 ? 0 : 1;
}
