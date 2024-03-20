#ifndef PARSINGTOOLS_H
#define PARSINGTOOLS_H
#define INITIAL_STRING_SIZE 64

#include "shellComponents.h"
#include <stdbool.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define PURPLE "\033[0;35m"     /* Purple */

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

typedef struct ListNode List;
typedef struct ListNode {
    char *t;
    List *next;
} List;

// Function prototypes from scanner.c
char *readInputLine(); 
List *newNode(char *s, int *start); 
List *newOperatorNode(char *s, int *start); 
char *matchIdentifier(char *s, int *start) ;


//Function prototypes from tokenList.c
List *getTokenList(char *s);
bool isEmpty(List *l);
void printList(List *l);
void freeTokenList(List *l);

//Function prototypes from operators.c
bool isOperatorCharacter(char c);
char *matchOperator(char *s, int *start);


// Function prototypes from parser.c
Command* parseInputLine();
bool parseCommand();
bool parseExecutable();
bool parseOptions();
bool parsePipeline();
bool parseChain();
bool parseBuiltIn();
bool parseRedirections();
bool parseFileName(char operator);



typedef struct shellData {
    List *tokenList;
    Command *commandList;
    Command *currentCommand;
    List *currentToken; 
    char* inputPath;
    char* outputPath;
    bool isPipeline; 
    int lastExitStatus; 
} shellData;

// In data.c
extern shellData *data;

shellData *getShellData(void);
void shellDataInit(char *inputLine);
void shellDataDestroy(void);
void printData();


#endif
