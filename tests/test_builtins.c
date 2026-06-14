/* Built-in tests: cd actually changes the cwd (the reason it must be a
   built-in), reports status, and is safe on failure; pwd/help report success.
   These link shell.c, which the native target already requires. */
#include <unistd.h>
#include <string.h>
#include "../shell.h"
#include "test.h"
#include "tests.h"

extern int lastExitStatus; /* defined in shell.c */

void run_builtins_tests(void) {
  SUITE("builtins (cd/pwd/help)");
  char orig[1024];
  char now[1024];
  CHECK(getcwd(orig, sizeof orig) != NULL, "can read the starting cwd");

  char *up[] = {"..", NULL};
  cdShell(up);
  CHECK_INT_EQ(lastExitStatus, 0, "cd .. succeeds");
  CHECK(getcwd(now, sizeof now) != NULL && strcmp(now, orig) != 0,
        "cd .. actually changed the cwd");

  char *back[] = {orig, NULL};
  cdShell(back);
  CHECK(getcwd(now, sizeof now) != NULL && strcmp(now, orig) == 0,
        "cd back to original restores the cwd");

  char *bad[] = {"/op_shell_no_such_dir_42", NULL};
  cdShell(bad);
  CHECK_INT_EQ(lastExitStatus, 1, "cd to a missing dir reports failure");
  CHECK(getcwd(now, sizeof now) != NULL && strcmp(now, orig) == 0,
        "a failed cd leaves the cwd unchanged");

  pwdShell(NULL);
  CHECK_INT_EQ(lastExitStatus, 0, "pwd succeeds");
  helpShell(NULL);
  CHECK_INT_EQ(lastExitStatus, 0, "help succeeds");
}
