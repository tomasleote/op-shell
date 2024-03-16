#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "parsingTools.h"

/**
 * Checks whether list \param l is empty.
 * @param l input list.
 * @return a bool denoting whether \param l is empty.
 */
bool isEmpty(List l) {
  return l == NULL;
}

/**
 * The function printList prints the tokens in a token list, separated by commas.
 * @param li the input list to be printed.
 */
void printList(List li) {
  if (li == NULL) return;
  printf("\"%s\"", li->t);
  li = li->next;
  while (li != NULL) {
    printf(", \"%s\"", li->t);
    li = li->next;
  }
  printf("\n");
}

/**
 * The function freeNode frees the memory of the node and of the string in the node.
 * @param node the node to be freed.
*/
void freeNode(List node) {
  free(node->t);
  free(node);
}

/**
 * The function freeTokenlist frees the memory of the nodes of the list, and of the strings
 * in the nodes.
 * @param li the starting node of a list.
 */
void freeTokenList(List li) {
  if (li == NULL) {
    return;
  }
  List next = li->next; // Save the next node before freeing the current node
  freeNode(li); // Free the current node
  freeTokenList(next); // Recurse into the next node
}

/**
 * The function tokenList reads an array and puts the tokens that are read in a list.
 * @param s input string.
 * @return a pointer to the beginning of the list.
 */
List getTokenList(char *s) {
  List lastNode = NULL;
  List node = NULL;
  List tl = NULL;
  int i = 0;
  int length = strlen(s);
  while (i < length) {
    if (isspace((unsigned char)s[i])) { 
      // spaces are skipped
      i++;
    }
    else {
      node = isOperatorCharacter(s[i]) ? newOperatorNode(s, &i) : newNode(s, &i);
      if (lastNode == NULL) { 
        // there is no list yet
        tl = node;
      }
      else { 
        // a list already exists; add current node at the end
        (lastNode)->next = node;
      }
      lastNode = node;
    }
  }
  return tl;
}

/**
 * The function newNode makes a new node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newNode(char *s, int *start) {
  List node = malloc(sizeof(*node));
  assert(node != NULL);
  node->next = NULL;
  node->t = matchIdentifier(s, start);
  return node;
}

/**
 * The function newOperatorNode makes a new operator node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List newOperatorNode(char *s, int *start) {
  List node = malloc(sizeof(*node));
  assert(node != NULL);
  node->next = NULL;
  node->t = matchOperator(s, start);
  return node; 
}