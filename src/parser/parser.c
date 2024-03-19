#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "shellComponents.h"
#include "parsingTools.h"

Command* currentCommandBeingParsed = NULL;

//TODO: Check this, might not be working. 
bool isValidSyntax() {
    bool openQuote = false;
    char quoteChar = '\0';
    List *tmp = data->currentToken; // Use the global currentToken for iteration

    while (tmp) {
      char *token = tmp->t; // Correctly dereferencing to get the token
      // Check for the presence of quotes in the token
      for (int i = 0; token && token[i] != '\0'; i++) {
        if (token[i] == '"' || token[i] == '\'') {
          if (!openQuote) {
          // Opening quote found
            openQuote = true;
            quoteChar = token[i];
          } else if (quoteChar == token[i]) {
            // Closing quote found
            openQuote = false;
            quoteChar = '\0'; // Reset quote character for next potential quote pair
          }
        }
      }
    tmp = tmp->next; // Move to the next token in the list
    }

    // If we've reached the end and a quote is still open, syntax is invalid
    if (openQuote) {
      printf("! Error: invalid syntax due to unclosed quote!\n");
      return false;
    }

    // If additional syntax checks are needed, implement them here

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
bool acceptToken(char *ident) {
  if (!data->currentToken)
    return false;
  if (strcmp(data->currentToken->t, ident) == 0) {
    // Advance the global currentToken after matching
    data->currentToken = data->currentToken->next;
    return true;
  }
  return false;
}

/**
 * The function parseRedirections parses a command according to the grammar:
 *
 * <command>        ::= <executable> <options>
 *
 * @return a bool denoting whether the command was parsed successfully.
 */
bool parseCommand() {
    bool parsed = (parseExecutable() && parseOptions());
    return parsed; 
}


/**
 * The function parseExecutable parses an executable.
 * @return a bool denoting whether the executable was parsed successfully.
 */
bool parseExecutable() {

  if (!isValidSyntax(data->currentToken)) {
    printf("Error: invalid syntax!\n");
    return false;
  }

  if (data->currentToken == NULL || isOperator(data->currentToken->t)) {
    return false;
  }

  char* executableName = data->currentToken->t;
  Command* newCmd = createCommand(executableName);
  newCmd->type = CMD_EXECUTABLE;
  currentCommandBeingParsed = newCmd;
  data->currentCommand = newCmd;

  if (newCmd == NULL) return false;
  
  
  if (data->commandList == NULL) {
        data->commandList = newCmd;
  } else {
    appendCommand(&(data->commandList), newCmd);
  }

  data->currentToken = data->currentToken->next;
  return true;
}

/**
 * The function parseOptions parses options.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the options were parsed successfully.
 */
bool parseOptions() {
  Command* currentCmd = data->currentCommand;
  
  if (currentCmd == NULL) return false;

  while (currentCmd->next != NULL) {
    currentCmd = currentCmd->next;
  }

  // TODO: store each tmp->t as an option, if any exist
  // TODO: Allocate memory for the array of strings DO NOT FORGET TO FREE LATER
  //cu->options = (char **)malloc(sizeof(char *) * command->optionCount);
  while (data->currentToken != NULL && !isOperator(data->currentToken->t)) {
    addOptionToCommand(currentCmd, data->currentToken->t);
    // Directly advance the global currentToken since we're reading each token as an option
    data->currentToken = data->currentToken->next;
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
bool parsePipeline() {

  if (!parseCommand()) {
    return false;
  }

  if (acceptToken("|")) {
    changeOperator(data->currentCommand, OP_PIPE);
    return parsePipeline();
  }

  return true;
}

/**
 * The function parseChain parses a chain according to the grammar:
 *
 * <chain>              ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @return a bool denoting whether the chain was parsed successfully.
 */
bool parseChain() {
  if (parseBuiltIn()) {
    return parseOptions();
  }
  if (parsePipeline()) {
    return parseRedirections();
  }
  return false;
}

/**
 * The function parseBuiltIn parses a builtin.
 * @return a bool denoting whether the builtin was parsed successfully.
 */
bool parseBuiltIn() {

  char *builtIns[] = {
      "exit",
      "status",
      "cd",
      NULL};

  for (int i = 0; builtIns[i] != NULL; i++) {
    if (acceptToken(builtIns[i])) {
      Command* newCmd = createCommand(builtIns[i]);
      newCmd->type = CMD_BUILTIN; // Set as built-in command
      currentCommandBeingParsed = newCmd;
      data->currentCommand = newCmd;
      appendCommand(&(data->commandList), newCmd);
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
 * @return a bool denoting whether the redirections were parsed successfully.
 */
bool parseRedirections() {
  if (isEmpty(data->currentToken)) {
    return true;
  }

  char operator = '<';

  if (acceptToken("<")) {
    if (!parseFileName(operator)) {
      return false;
    } else if (acceptToken(">")) {
      operator = '>';
      return parseFileName(operator);
    } else {
      return true;
    }
  } else if (acceptToken(">")) {
    operator = '>';
    if (!parseFileName(operator)) {
      return false;
    } else if (acceptToken("<")) {
      operator = '<';
      return parseFileName(operator);
    } else {
      return true;
    }
  }

  return true;
}

/**
 * The function parseFileName parses a filename.
 * @return a bool denoting whether the filename was parsed successfully.
 */
bool parseFileName(char operatorChar) {

  if (data->currentToken == NULL || data->currentToken->t == NULL) {
    return false;
  }

  char *fileName = data->currentToken->t;
  if (operatorChar == '<' && fileName != NULL) {
    data->currentCommand->inputFile = strdup(fileName);
  } else if (operatorChar == '>' && fileName != NULL) {
    data->currentCommand->outputFile = strdup(fileName);
  }
  
  data->currentToken = data->currentToken->next;
  return true;
}

/**
 * The function parseInputLineInternal parses an inputline according to the grammar:
 * <inputline>      ::= <chain> & <inputline>
 *                   | <chain> && <inputline>
 *                   | <chain> || <inputline>
 *                   | <chain> ; <inputline>
 *                   | <chain>
 *                   | <empty>
 * @return a bool denoting whether the inputline was parsed successfully.
*/
bool parseInputLineInternal() {

  while (data->currentToken != NULL) {  
    if (!parseChain() && data->currentToken != NULL) {
      if (acceptToken("&") || acceptToken("&&")) {
        changeOperator(data->currentCommand, OP_AND);
        continue;
      } else if (acceptToken("||")) {
          changeOperator(data->currentCommand, OP_OR);
          continue; 
      } else if (acceptToken(";")) {
          changeOperator(data->currentCommand, OP_SEQ);
          continue;
      }
    }
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
 * @return a bool denoting whether the inputline was parsed successfully.
 */
Command *parseInputLine() {
  currentCommandBeingParsed = NULL;

  // Call the internal parsing function with the head pointer
  parseInputLineInternal();
  return (data->commandList);
}
