#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "shellComponents.h"
#include "parsingTools.h"

//TODO: Finish implementing this, not working yet
bool isValidSyntax(List lp) {
    bool openQuote = false;
    char quoteChar = '\0';

    while (lp != NULL) {
        char *token = lp->t; // Correctly dereferencing to get the token
        // Check for the presence of quotes in the token
        for (int i = 0; token[i] != '\0'; i++) {
            if (token[i] == '"' || token[i] == '\'') {
                if (!openQuote) {
                    // Opening quote found
                    openQuote = true;
                    quoteChar = token[i];
                } else if (quoteChar == token[i]) {
                    // Closing quote found
                    openQuote = false;
                }
            }
        }

        lp = lp->next; // Move to the next token in the list
    }

    // If we've reached the end and a quote is still open, syntax is invalid
    if (openQuote) {
        printf("! Error: invalid syntax!\n");
        return false;
    }

    //printf("Syntax is valid!\n");
    return true;
}


/**
 * Checks whether the input string \param s is an operator.
 * @param s input string.
 * @return a bool denoting whether the current string is an operator.
 */
bool isOperator(char *s) {
  
  char *operators[] = {
      "&",
      "&&",
      "||",
      ";",
      "<",
      ">",
      "|",
      NULL};

  for (int i = 0; operators[i] != NULL; i++) {
    if (strcmp(s, operators[i]) == 0) {
      return true;
    } 
  }
  
  return false;
}

/**
 * The function acceptToken checks whether the current token matches a target identifier,
 * and goes to the next token if this is the case.
 * @param lp List pointer to the start of the tokenlist.
 * @param ident target identifier
 * @return a bool denoting whether the current token matches the target identifier.
 */
bool acceptToken(List *lp, char *ident) {
  if (*lp != NULL && strcmp(((*lp)->t), ident) == 0) {
    *lp = (*lp)->next;
    return true;
  }

  return false;
}

/**
 * The function parseRedirections parses a command according to the grammar:
 *
 * <command>        ::= <executable> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the command was parsed successfully.
 */
bool parseCommand(List *lp, Command** head) {
    return parseExecutable(lp, head) && parseOptions(lp, head);
}


/**
 * The function parseExecutable parses an executable.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the executable was parsed successfully.
 */
bool parseExecutable(List *lp, Command **head) {

  if (!isValidSyntax(*lp)) {
    perror("Invalid syntax!\n");
    return false;
  }

  char* executableName = (*lp)->t;
  Command* newCmd = createCommand(executableName);

  if (newCmd == NULL) {
    freeCommand(newCmd);
    return false;
  }
  
  if (*head == NULL) {
    *head = newCmd;
  } else {
    appendCommand(head, newCmd);
  }

  (*lp) = (*lp)->next;  

  return true;
}

/**
 * The function parseOptions parses options.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the options were parsed successfully.
 */
bool parseOptions(List *lp, Command** head) {
  if (*head == NULL) return false;

  Command* currentCmd = *head;

  while (currentCmd->next != NULL) {
    currentCmd = currentCmd->next;
  }

  // TODO: store each (*lp)->t as an option, if any exist
  while (*lp != NULL && !isOperator((*lp)->t)) {
    addOptionToCommand(currentCmd, (*lp)->t);
    (*lp) = (*lp)->next;
  }

  return true;
}


/**
 * The function parsePipeline parses a pipeline according to the grammar:
 *
 * <pipeline>           ::= <command> "|" <pipeline>
 *                       | <command>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the pipeline was parsed successfully.
 */
bool parsePipeline(List *lp, Command** head) {
  if (!parseCommand(lp, head)) {
    return false;
  }

  if (acceptToken(lp, "|")) {
    return parsePipeline(lp, head);
  }

  return true;
}

/**
 * The function parseChain parses a chain according to the grammar:
 *
 * <chain>              ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the chain was parsed successfully.
 */
bool parseChain(List *lp, Command** head) {
  if (parseBuiltIn(lp, head)) {
    return parseOptions(lp, head);
  }
  if (parsePipeline(lp, head)) {
    return parseRedirections(lp);
  }
  return false;
}

/**
 * The function parseBuiltIn parses a builtin.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the builtin was parsed successfully.
 */
bool parseBuiltIn(List *lp, Command** head) {

  char *builtIns[] = {
      "exit",
      "status",
      "cd",
      NULL};

  for (int i = 0; builtIns[i] != NULL; i++) {
    if (acceptToken(lp, builtIns[i])) {
      Command* newCmd = createCommand(builtIns[i]);
      newCmd->type = CMD_BUILTIN; // Set as built-in command

      if (*head == NULL) {
        *head = newCmd;
      } else {
        appendCommand(head, newCmd);
      }

      return true;
    }
  }

  return false;
}

/**
 * The function parseRedirections parses redirections according to the grammar:
 *
 * <redirections>       ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the redirections were parsed successfully.
 */
bool parseRedirections(List *lp) {
  if (isEmpty(*lp)) {
    return true;
  }

  if (acceptToken(lp, "<")) {

    if (!parseFileName(lp)) {
        return false;
    } else if (acceptToken(lp, ">")) { 
      return parseFileName(lp);
    } else {
      return true;
    }
  } else if (acceptToken(lp, ">")) {
    
    if (!parseFileName(lp)) {
      return false;
    } else if (acceptToken(lp, "<")) {
      return parseFileName(lp);
    } else {
      return true;
    }
  }

  return true;
}

/**
 * The function parseFileName parses a filename.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the filename was parsed successfully.
 */
bool parseFileName(List *lp) {
  char *fileName = (*lp)->t;
  if (fileName == NULL) {
    return false;
  }
  (*lp) = (*lp)->next;
  return true;
}

bool parseInputLineInternal(List* lp, Command** head) {
    if (isEmpty(*lp)) {
        return true;
    }

    if (!parseChain(lp, head)) {
        return false;
    }

    if (acceptToken(lp, "&") || acceptToken(lp, "&&")) {


        return parseInputLineInternal(lp, head);
    } else if (acceptToken(lp, "||")) {



        return parseInputLineInternal(lp, head);
    } else if (acceptToken(lp, ";")) {

      
        return parseInputLineInternal(lp, head);
    }
  return false; 
}

/**
 * The function parseInputLine parses an inputline according to the grammar:
 *
 * <inputline>      ::= <chain> & <inputline>
 *                   | <chain> && <inputline>
 *                   | <chain> || <inputline>
 *                   | <chain> ; <inputline>
 *                   | <chain>
 *                   | <empty>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the inputline was parsed successfully.
 */
Command *parseInputLine(List *lp, int *parsedSuccessfully) {
  Command* head = NULL;

  // Call the internal parsing function with the head pointer
  parseInputLineInternal(lp, &head);
  *parsedSuccessfully = 1;
  return (head);
}
