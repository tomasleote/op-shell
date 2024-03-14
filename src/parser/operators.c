#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "parsingTools.h"

// Static definitions of operator strings
static char op_and[] = "&";
static char op_or[] = "|";
static char op_semi[] = ";";
static char op_redirect_in[] = "<";
static char op_redirect_out[] = ">";
static char op_double_and[] = "&&";
static char op_double_or[] = "||";

/**
 * The function isOperatorCharacter checks whether the input paramater \param c is an operator.
 * @param c input character.
 * @return a bool denoting whether \param c is an operator.
 */
bool isOperatorCharacter(char c) {
  return c == '&' || c == '|' || c == ';' || c == '<' || c == '>';
}

/**
 * Reads an operator in string \param s starting at index \param start.
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a pointer to the start of the operator string.
 */
char *matchOperator(char *s, int *start) {

    char *op = NULL; // This will point to the correct static string
    int offset = 0;

    // Check for double character operators first
    if (strncmp(&s[*start], "&&", 2) == 0) {
        op = op_double_and;
        offset = 2;
    } else if (strncmp(&s[*start], "||", 2) == 0) {
        op = op_double_or;
        offset = 2;
    }
    // Then check for single character operators
    else if (s[*start] == '&') {
        op = op_and;
        offset = 1;
    } else if (s[*start] == '|') {
        op = op_or;
        offset = 1;
    } else if (s[*start] == ';') {
        op = op_semi;
        offset = 1;
    } else if (s[*start] == '<') {
        op = op_redirect_in;
        offset = 1;
    } else if (s[*start] == '>') {
        op = op_redirect_out;
        offset = 1;
    }

    *start += offset; // Advance the start pointer by the length of the operator
    return op;
}

