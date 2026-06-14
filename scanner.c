#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "scanner.h"

// malloc/realloc wrappers: abort on allocation failure (matches command.c).
static void *xmalloc(size_t n)
{
  void *p = malloc(n);
  if (p == NULL) { perror("malloc"); exit(EXIT_FAILURE); }
  return p;
}

static void *xrealloc(void *p, size_t n)
{
  void *q = realloc(p, n);
  if (q == NULL) { perror("realloc"); exit(EXIT_FAILURE); }
  return q;
}

/**
 * Reads an inputline from stdin.
 * @return a string containing the inputline.
 */
char *readInputLine()
{
  // TODO: set string size dynamically
  int strLen = INITIAL_STRING_SIZE;
  int c = getchar();
  int i = 0;

  if (c == EOF)
  { // EOF (Ctrl+D / closed stdin) on an empty line ends the REPL
    return NULL;
  }

  char *s = xmalloc((strLen + 1) * sizeof(*s));

  bool quoteStarted = false;
  while (c != EOF && (c != '\n' || quoteStarted))
  { // Ensure that newlines in strings are accepted; stop cleanly at EOF
    if (c == '\"')
    {
      quoteStarted = !quoteStarted;
    }
    s[i++] = c;

    if (i >= strLen)
    { // Resize the string if necessary
      strLen = 2 * strLen;
      s = xrealloc(s, (strLen + 1) * sizeof(*s));
    }
    c = getchar();
  }
  s[i] = '\0';
  return s;
}

/**
 * The function isOperatorCharacter checks whether the input paramater \param c is an operator.
 * @param c input character.
 * @return a bool denoting whether \param c is an operator.
 */
bool isOperatorCharacter(char c)
{
  return c == '&' || c == '|' || c == ';' || c == '<' || c == '>';
}

/**
 * Reads an identifier in string \param s starting at index \param start.
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a pointer to the start of the identifier string
 */
char *matchIdentifier(char *s, int *start)
{
  int strLen = INITIAL_STRING_SIZE;
  int pos = 0, offset = 0;

  char *ident = xmalloc((strLen + 1) * sizeof(*ident));

  bool quoteStarted = false;
  size_t lenS = strlen(s);
  // Bound by lenS first so an unterminated quote cannot read past the '\0'.
  while (*start + offset < (int)lenS &&
         (quoteStarted || (!isspace((unsigned char)s[*start + offset]) && !isOperatorCharacter(s[*start + offset]))))
  { // Ensure that whitespace in strings is accepted
    if (s[*start + offset] == '\"')
    { // Strip the quotes from the input before storing in the identifier
      quoteStarted = !quoteStarted;
      offset++;
      continue;
    }
    ident[pos++] = s[*start + offset++];
    if (pos >= strLen)
    { // Resize the string if necessary
      strLen = 2 * strLen;
      ident = xrealloc(ident, (strLen + 1) * sizeof(*ident));
    }
  }
  ident[pos] = '\0';
  *start = *start + offset;
  return ident;
}

/**
 * The function newNode makes a new node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newNode(char *s, int *start)
{
  List node = xmalloc(sizeof(*node));
  node->next = NULL;
  node->t = matchIdentifier(s, start);
  return node;
}

/**
 * Reads an operator in string \param s starting at index \param start.
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a pointer to the start of the operator string.
 */
char *matchOperator(char *s, int *start)
{
  int strLen = 2; // the operator consists of *at most* 2 characters
  int pos = 0, offset = 0;

  char *op = xmalloc((strLen + 1) * sizeof(*op));

  while (pos < strLen && isOperatorCharacter(s[*start + offset]))
  { // cap at strLen: operators are at most 2 chars, prevents heap overflow on "<<<" etc.
    op[pos++] = s[*start + offset++];
  }
  op[pos] = '\0';
  *start = *start + offset;
  return op;
}

/**
 * The function newOperatorNode makes a new operator node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newOperatorNode(char *s, int *start)
{
  List node = xmalloc(sizeof(*node));
  node->next = NULL;
  node->t = matchOperator(s, start);
  return node;
}

/**
 * The function tokenList reads an array and puts the tokens that are read in a list.
 * @param s input string.
 * @return a pointer to the beginning of the list.
 */
List getTokenList(char *s)
{
  List lastNode = NULL;
  List node = NULL;
  List tl = NULL;
  int i = 0;
  int length = strlen(s);
  while (i < length)
  {
    if (isspace((unsigned char)s[i]))
    { // spaces are skipped
      i++;
    }
    else
    {
      node = isOperatorCharacter(s[i]) ? newOperatorNode(s, &i) : newNode(s, &i);
      if (lastNode == NULL)
      { // there is no list yet
        tl = node;
      }
      else
      { // a list already exists; add current node at the end
        (lastNode)->next = node;
      }
      lastNode = node;
    }
  }
  return tl;
}

/**
 * Checks whether list \param l is empty.
 * @param l input list.
 * @return a bool denoting whether \param l is empty.
 */
bool isEmpty(List l)
{
  return l == NULL;
}

/**
 * The function printList prints the tokens in a token list, separated by commas.
 * @param li the input list to be printed.
 */
void printList(List li)
{
  if (li == NULL)
    return;
  printf("\"%s\"", li->t);
  li = li->next;
  while (li != NULL)
  {
    printf(", \"%s\"", li->t);
    li = li->next;
  }
  printf("\n");
}

/**
 * The function freeTokenlist frees the memory of the nodes of the list, and of the strings
 * in the nodes.
 * @param li the starting node of a list.
 */
void freeTokenList(List li)
{
  if (li == NULL)
  {
    return;
  }
  free(li->t);
  freeTokenList(li->next);
  free(li);
}
