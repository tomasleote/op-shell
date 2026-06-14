/* Parser tests: built-in recognition, options, pipelines, operators, syntax
   error propagation, and null safety. */
#include <stdlib.h>
#include <string.h>
#include "../scanner.h"
#include "../parser.h"
#include "../command.h"
#include "test.h"
#include "tests.h"

static void free_cmds(Command *c) {
  while (c != NULL) {
    Command *n = c->next;
    freeCommand(c);
    c = n;
  }
}

/* Parses one line; reports whether the parse succeeded (*ok) and whether every
   token was consumed (*consumed). The returned command list is independent of
   the token list and is owned by the caller. */
static Command *parse_line(const char *s, int *ok, int *consumed) {
  char *dup = strdup(s);
  List toks = getTokenList(dup);
  List cur = toks;
  Command *c = parseInputLine(&cur, ok);
  if (consumed) *consumed = (cur == NULL);
  freeTokenList(toks);
  free(dup);
  return c;
}

static int count_cmds(Command *c) {
  int n = 0;
  for (; c != NULL; c = c->next) n++;
  return n;
}

void run_parser_tests(void) {
  SUITE("parser");
  int ok, consumed;

  {
    Command *c = parse_line("exit", &ok, &consumed);
    CHECK(ok && consumed, "'exit' parses and consumes all tokens");
    CHECK(c != NULL && c->type == CMD_BUILTIN, "exit is a builtin");
    free_cmds(c);
  }

  {
    Command *c = parse_line("cd /tmp", &ok, &consumed);
    CHECK(c != NULL && c->type == CMD_BUILTIN, "cd is a builtin");
    CHECK_STR_EQ(c ? c->command : NULL, "cd", "command name is cd");
    CHECK_INT_EQ(c ? c->optionCount : -1, 1, "cd has one option");
    CHECK_STR_EQ((c && c->optionCount > 0) ? c->options[0] : NULL, "/tmp",
                 "cd argument captured");
    free_cmds(c);
  }

  {
    Command *c = parse_line("pwd", &ok, &consumed);
    CHECK(c != NULL && c->type == CMD_BUILTIN, "pwd is a builtin");
    free_cmds(c);
    c = parse_line("help", &ok, &consumed);
    CHECK(c != NULL && c->type == CMD_BUILTIN, "help is a builtin");
    free_cmds(c);
  }

  {
    Command *c = parse_line("ls -la", &ok, &consumed);
    CHECK(c != NULL && c->type == CMD_EXTERNAL, "ls is an external command");
    CHECK_INT_EQ(c ? c->optionCount : -1, 1, "ls has one option");
    CHECK_STR_EQ((c && c->optionCount > 0) ? c->options[0] : NULL, "-la",
                 "ls option captured");
    free_cmds(c);
  }

  {
    Command *c = parse_line("ls | wc", &ok, &consumed);
    CHECK_INT_EQ(count_cmds(c), 2, "pipeline yields two commands");
    free_cmds(c);
  }

  {
    Command *c = parse_line("echo a ; echo b", &ok, &consumed);
    CHECK_INT_EQ(count_cmds(c), 2, "; separates two commands");
    free_cmds(c);
  }

  {
    Command *c = parse_line("a && b", &ok, &consumed);
    CHECK_INT_EQ(count_cmds(c), 2, "&& separates two commands");
    free_cmds(c);
  }

  {
    /* Empty input is a valid (empty) line. */
    Command *c = parse_line("", &ok, &consumed);
    CHECK(ok && c == NULL, "empty line parses to no commands");
    free_cmds(c);
  }

  {
    /* Regression (bugs 3 + 5): a trailing pipe with no command must NOT crash
       and must report failure rather than silently succeeding. */
    Command *c = parse_line("ls |", &ok, &consumed);
    CHECK_INT_EQ(ok, 0, "trailing pipe reports a parse failure");
    free_cmds(c);
  }

  {
    Command *c = parse_line("echo \"hello world\"", &ok, &consumed);
    CHECK_INT_EQ(c ? c->optionCount : -1, 1, "quoted arg is one option");
    CHECK_STR_EQ((c && c->optionCount > 0) ? c->options[0] : NULL,
                 "hello world", "quoted arg keeps its space");
    free_cmds(c);
  }
}
