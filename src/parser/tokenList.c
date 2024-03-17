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
bool isEmpty(List *l) {
  return l == NULL;
}

/**
 * The function printList prints the tokens in a token list, separated by commas.
 * @param li the input list to be printed.
 */
void printList(List *li) {
  if (li == NULL) return;
	printf("%sToken list:\n", MAGENTA);
  printf("\"%s\"", li->t);
  li = li->next;
  while (li != NULL) {
    printf(", \"%s\"", li->t);
    li = li->next;
  }
  printf("%s\n", WHITE);
}

/**
 * The function freeNode frees the memory of the node and of the string in the node.
 * @param node the node to be freed.
*/

/**
 * The function freeTokenlist frees the memory of the nodes of the list, and of the strings
 * in the nodes.
 * @param li the starting node of a list.
 */
void freeTokenList(List *li) {
  if (li == NULL) {
    return;
  } else if (isOperatorCharacter(*li->t) && li->next != NULL) {
    List *next = li->next;
    free(li);
    freeTokenList(next);
  } else {
    List *next = li->next;
    free(li->t);
    free(li);
    freeTokenList(next);
  }
}

/**
 * The function tokenList reads an array and puts the tokens that are read in a list.
 * @param s input string.
 * @return a pointer to the beginning of the list.
 */
List *getTokenList(char *s) {
	List *head = NULL;
  List *node = NULL;
  List *tl = NULL;
  int i = 0;
  int length = strlen(s);
  while (i < length) {
    if (isspace((unsigned char)s[i]))
      i++;
    else {
      node = isOperatorCharacter(s[i]) ? newOperatorNode(s, &i) : newNode(s, &i);
      if (!node)
				break;
			if (!head) {
        tl = node;			//tl pointer to newly created node	
				head = tl;			//saving the pointer of the head of the tl
			}
      else {
				tl->next = node;
				tl = tl->next;
			}
    }
  }
  tl->next = NULL;
	tl = head;
  return tl;
}

/**
 * The function newNode makes a new node for the token list and fills it with the token that
 * has been read. Precondition: !isspace(a[*ip]).
 * @param s input string.
 * @param start starting index in string \param s.
 * @return a list node that contains the current token.
 */
List *newNode(char *s, int *start) {
  List *node;
	node = malloc(sizeof(List));
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
List *newOperatorNode(char *s, int *start) {
  List *node = malloc(sizeof(*node));
  assert(node != NULL);
  node->next = NULL;
  node->t = matchOperator(s, start);
  return node; 
}