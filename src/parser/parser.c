#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "shellComponents.h"
#include "parsingTools.h"

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
    data->currentToken = NULL;
    return false;
  }

  if (data->currentToken == NULL || isOperator(data->currentToken->t)) {
    return false;
  }

  char* executableName = data->currentToken->t;
  Command* newCmd = createCommand(executableName);
  newCmd->type = CMD_EXECUTABLE;
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

  while (data->currentToken != NULL && !isOperator(data->currentToken->t)) {
    addOptionToCommand(currentCmd, data->currentToken->t);
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
    data->isPipeline = true;
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
    data->inputPath = strdup(fileName);
  } else if (operatorChar == '>' && fileName != NULL) {
    data->outputPath = strdup(fileName);
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
  parseInputLineInternal();
  return (data->commandList);
}
