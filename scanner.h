#ifndef SCANNER_H
#define SCANNER_H

#define INITIAL_STRING_SIZE 10

#ifndef GRAMMAR_TOKENS_H
#define GRAMMAR_TOKENS_H

// Define token types according to the shell grammar
typedef enum {
    TOKEN_EXECUTABLE,  // Represents an executable path or file in $PATH
    TOKEN_BUILTIN,     // Represents a built-in command
    TOKEN_OPTION,      // Represents command options or parameters
    TOKEN_PIPE,        // Represents |
    TOKEN_REDIRECT_IN, // Represents input redirection <
    TOKEN_REDIRECT_OUT, // Represents output redirection >
    TOKEN_AND,         // Represents background execution &
    TOKEN_AND_AND,     // Represents logical AND &&
    TOKEN_OR_OR,       // Represents logical OR ||
    TOKEN_SEMICOLON,   // Represents command separator ;
    TOKEN_STRING,      // Represents a string (enclosed in "")
    TOKEN_EMPTY,       // Represents an empty token or end of input
    // Add more token types as needed for future features
} TokenType;

typedef struct Token {
    TokenType type;
    char *value; // The actual text of the token, useful for executables, options, filenames, etc.
} Token;


typedef struct ListNode *List;

typedef struct ListNode {
    char *t;
    List next;
} ListNode;


char *readInputLine();

List getTokenList(char *s);

bool isEmpty(List l);

void printList(List l);

void freeTokenList(List l);

#endif
#endif 