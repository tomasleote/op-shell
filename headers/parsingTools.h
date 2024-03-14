#ifndef PARSINGTOOLS_H
#define PARSINGTOOLS_H
#define INITIAL_STRING_SIZE 64

#include "shellComponents.h"
#include <stdbool.h>

// Token and list definitions from scanner.c
typedef enum {
    TOKEN_EXECUTABLE,
    TOKEN_BUILTIN,
    TOKEN_OPTION,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_AND,
    TOKEN_AND_AND,
    TOKEN_OR_OR,
    TOKEN_SEMICOLON,
    TOKEN_STRING,
    TOKEN_EMPTY,
} TokenType;

typedef struct Token {
    TokenType type;
    char *value;
} Token;

typedef struct ListNode *List;

typedef struct ListNode {
    char *t;
    List next;
} ListNode;

// Function prototypes from scanner.c
char *readInputLine();
List getTokenList(char *s);
bool isEmpty(List l);
void printList(List l);
void freeTokenList(List l);

// Function prototypes from parser.c
Command* parseInputLine(List *lp, int *parsedSuccessfully);
bool parseCommand(List *lp, Command** head);
bool parseExecutable(List *lp, Command **head);
bool parseOptions(List *lp, Command** head);
bool parsePipeline(List *lp, Command** head);
bool parseChain(List *lp, Command** head);
bool parseBuiltIn(List *lp, Command **head);
bool parseRedirections(List *lp);
bool parseFileName(List *lp);

#endif
