/* Scanner tests: tokenization plus regressions for the fixed bugs. */
#include <stdlib.h>
#include <string.h>
#include "../scanner.h"
#include "test.h"
#include "tests.h"

/* getTokenList does not retain its argument (tokens are freshly allocated),
   so the duplicate can be freed immediately after tokenizing. */
static List tok(const char *s) {
  char *dup = strdup(s);
  List l = getTokenList(dup);
  free(dup);
  return l;
}

static int count_tokens(List l) {
  int n = 0;
  for (; l != NULL; l = l->next) n++;
  return n;
}

static const char *nth(List l, int n) {
  for (int i = 0; l != NULL && i < n; i++) l = l->next;
  return l ? l->t : NULL;
}

void run_scanner_tests(void) {
  SUITE("scanner");

  {
    List l = tok("ls -la");
    CHECK_INT_EQ(count_tokens(l), 2, "two tokens for 'ls -la'");
    CHECK_STR_EQ(nth(l, 0), "ls", "first token is ls");
    CHECK_STR_EQ(nth(l, 1), "-la", "second token is -la");
    freeTokenList(l);
  }

  {
    List l = tok("  ls   -l  ");
    CHECK_INT_EQ(count_tokens(l), 2, "surrounding/extra spaces are skipped");
    CHECK_STR_EQ(nth(l, 0), "ls", "trimmed first token");
    freeTokenList(l);
  }

  {
    List l = tok("a|b");
    CHECK_INT_EQ(count_tokens(l), 3, "pipe splits into three tokens");
    CHECK_STR_EQ(nth(l, 1), "|", "operator token is |");
    freeTokenList(l);
  }

  {
    List l = tok("a && b || c ; d");
    CHECK_INT_EQ(count_tokens(l), 7, "two-char and single-char operators");
    CHECK_STR_EQ(nth(l, 1), "&&", "&& tokenized as one operator");
    CHECK_STR_EQ(nth(l, 3), "||", "|| tokenized as one operator");
    CHECK_STR_EQ(nth(l, 5), ";", "; tokenized as operator");
    freeTokenList(l);
  }

  {
    List l = tok("echo \"hello world\"");
    CHECK_INT_EQ(count_tokens(l), 2, "quoted text stays a single token");
    CHECK_STR_EQ(nth(l, 1), "hello world", "quotes stripped, space kept");
    freeTokenList(l);
  }

  {
    /* Empty and whitespace-only input yield an empty list. */
    List a = tok("");
    List b = tok("    ");
    CHECK(a == NULL, "empty input gives empty token list");
    CHECK(b == NULL, "whitespace-only input gives empty token list");
    freeTokenList(a);
    freeTokenList(b);
  }

  {
    /* Regression (bug 12): an unterminated quote must terminate and stay
       bounded, not read past the end / loop forever. Reaching this assert at
       all proves the scanner returned. */
    List l = tok("echo \"abc");
    CHECK_INT_EQ(count_tokens(l), 2, "unterminated quote terminates safely");
    CHECK_STR_EQ(nth(l, 1), "abc", "content up to end is captured");
    freeTokenList(l);
  }

  {
    /* Regression (bug 2): runs of operator chars must not overflow the 2-byte
       operator buffer. No crash and a usable list is the assertion. */
    List l = tok("a<<<b");
    CHECK(l != NULL, "operator run does not overflow / crash");
    CHECK_STR_EQ(nth(l, 0), "a", "identifier before operator run intact");
    freeTokenList(l);
  }

  {
    /* Regression (bug 4): high (UTF-8) bytes are not misclassified as space or
       operator. "caf\xC3\xA9" is "cafe-acute" in UTF-8. */
    List l = tok("caf\xC3\xA9 ok");
    CHECK_INT_EQ(count_tokens(l), 2, "UTF-8 word is a single token");
    CHECK_STR_EQ(nth(l, 0), "caf\xC3\xA9", "UTF-8 bytes preserved in token");
    freeTokenList(l);
  }
}
