#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "parsingTools.h"

/**
 * Reads an inputline from stdin.
 * @return a string containing the inputline.
 */
char *readInputLine() {
  // TODO: set string size dynamically
  int strLen = INITIAL_STRING_SIZE;
  int c = getchar();

  if (c == EOF && feof(stdin)) { // Check if EOF is the first character read
    return NULL; // Signal EOF to the caller
  }

  int i = 0;

  char *s = malloc((strLen + 1) * sizeof(*s));
  assert(s != NULL);

  bool quoteStarted = false;
  while (c != '\n' || quoteStarted) { // Ensure that newlines in strings are accepted
    if (c == '\"') {
      quoteStarted = !quoteStarted;
    }
    s[i++] = c;

    if (i >= strLen) { // Resize the string if necessary
      strLen = 2 * strLen;
      s = realloc(s, (strLen + 1) * sizeof(*s));
      assert(s != NULL);
    }
    c = getchar();
  }
  s[i] = '\0';
  return s;
}

/**
 * Reads an identifier in string \param s starting at index \param start.
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a pointer to the start of the identifier string
 */
char *matchIdentifier(char *s, int *start) {
  int strLen = INITIAL_STRING_SIZE;
  int pos = 0, offset = 0;

  char *ident = malloc((strLen + 1) * sizeof(*ident));
  assert(ident != NULL);

  bool quoteStarted = false;
  size_t lenS = strlen(s);
  while ((*start + offset <= lenS && !isspace((unsigned char)s[*start + offset]) && !isOperatorCharacter(s[*start + offset])) || quoteStarted) { // Ensure that whitespace in strings is accepted
    if (s[*start + offset] == '\"') { 
    // Strip the quotes from the input before storing in the identifier
      quoteStarted = !quoteStarted;
      offset++;
      continue;
    }
    ident[pos++] = s[*start + offset++];
    if (pos >= strLen) { 
    // Resize the string if necessary
      strLen = 2 * strLen;
      ident = realloc(ident, (strLen + 1) * sizeof(*ident));
      assert(ident != NULL);
    }
  }
  ident[pos] = '\0';
  *start = *start + offset;
  return ident;
}




