#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"
#include <unistd.h>
#include <sys/wait.h>

/*
void executeCommand(Command* cmd) {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        // Convert cmd->options to the format expected by execvp
        char** args = prepareArgsForExecvp(cmd);
        execvp(cmd->command, args);
        // If execvp returns, an error occurred
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to complete
    } else {
        // Handle error in fork()
        perror("fork");
        exit(EXIT_FAILURE);
    }
}*/


/*
  Builtin function implementations. Check this later
*
int cd(char **args)
{
  printf("Changing directory\n");
  if (args[1] == NULL)
  {
    fprintf(stderr, "expected argument to \"cd\"\n");
  }
  else
  {
    if (chdir(args[1]) != 0)
    {
      perror("shell");
    }
  }
  return 1;
}*/

int exitShell(char **args)
{
  printf("Exiting shell\n");
  exit(0);
}

/**
 * Checks whether the input string \param s is an operator.
 * @param s input string.
 * @return a bool denoting whether the current string is an operator.
 */
bool isOperator(char *s)
{
  // NULL-terminated array makes it easy to expand this array later
  // without changing the code at other places.
  char *operators[] = {
      "&",
      "&&",
      "||",
      ";",
      "<",
      ">",
      "|",
      NULL};

  for (int i = 0; operators[i] != NULL; i++)
  {
    if (strcmp(s, operators[i]) == 0)
      return true;
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
bool acceptToken(List *lp, char *ident)
{
  if (*lp != NULL && strcmp(((*lp)->t), ident) == 0)
  {
    *lp = (*lp)->next;
    return true;
  }
  return false;
}

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtinStr[] = {
    //"cd",
    "exit"};

int numBuiltins()
{
  return sizeof(builtinStr) / sizeof(char *);
}

int (*builtinFunc[])(char **) = {
    //&cd,
    &exitShell
  };

parseAndExecute(List *lp) { 

}

/**
 * The function parseExecutable parses an executable.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the executable was parsed successfully.
 */
bool parseExecutable(List *lp, Command **head)
{
  // TODO: Determine whether to accept parsing an executable here.
  //
  // It is not recommended to check for existence of the executable already
  // here, since then it'll be hard to continue parsing the rest of the input
  // line (command execution should continue after a "not found" command),
  // it'll also be harder to print the correct error message.
  //
  // Instead, we recommend to just do a syntactical check here, which makes
  // more sense, and defer the binary existence check to the runtime part
  // you'll write later.

  // for now, not optimal
  char* executableName = (*lp)->t;
  Command* newCmd = createCommand(executableName);
  printf("newCmd: %s\n", newCmd->command);

  printf("head pointer before if: %p\n", *head);
  if (*head == NULL) {
    *head = newCmd;
    printf("head pointer inside if: %p\n", *head);
  } else {
    appendCommand(head, newCmd);
  }

  (*lp) = (*lp)->next;  

  return true;

  // the ta's advice was to launch the command either
  // after parsing the command, not executable
}

/**
 * The function parseOptions parses options.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the options were parsed successfully.
 */
bool parseOptions(List *lp, Command** head)
{
  if (*head == NULL) return false;

  Command* currentCmd = *head;
  while (currentCmd->next != NULL) {
    currentCmd = currentCmd->next;
  }

  // TODO: store each (*lp)->t as an option, if any exist
  while (*lp != NULL && !isOperator((*lp)->t))
  {
    addOptionToCommand(currentCmd, (*lp)->t);
    printf("added option: %s\n", (*lp)->t);
    (*lp) = (*lp)->next;
  }
  return true;
}

/**
 * The function parseRedirections parses a command according to the grammar:
 *
 * <command>        ::= <executable> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the command was parsed successfully.
 */
bool parseCommand(List *lp, Command** head)
{
  printf("entered parseCommand\n");
  return parseExecutable(lp, head) && parseOptions(lp, head);
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
bool parsePipeline(List *lp, Command** head)
{
  printf("entered parsePipeline\n");
  if (!parseCommand(lp, head))
  {
    return false;
  }

  if (acceptToken(lp, "|"))
  {
    return parsePipeline(lp, head);
  }

  return true;
}

/**
 * The function parseFileName parses a filename.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the filename was parsed successfully.
 */
bool parseFileName(List *lp)
{
  // TODO: Process the file name appropriately
  printf ("entered parseFileName\n");
  char *fileName = (*lp)->t;
  if (fileName == NULL)
  {
    return false;
  }
  lp = (*lp)->next;
  return true;
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
bool parseRedirections(List *lp)
{
  printf("entered parseRedirections\n");
  if (isEmpty(*lp))
  {
    return true;
  }

  if (acceptToken(lp, "<"))
  {
    if (!parseFileName(lp))
      return false;
    if (acceptToken(lp, ">"))
      return parseFileName(lp);
    else
      return true;
  }
  else if (acceptToken(lp, ">"))
  {
    if (!parseFileName(lp))
      return false;
    if (acceptToken(lp, "<"))
      return parseFileName(lp);
    else
      return true;
  }

  return true;
}

/**
 * The function parseBuiltIn parses a builtin.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the builtin was parsed successfully.
 */
bool parseBuiltIn(List *lp, Command** head)
{

  //
  // TODO: Implement the logic for these builtins, and extend with
  // more builtins down the line
  //

  // NULL-terminated array makes it easy to expand this array later
  // without changing the code at other places.
  char *builtIns[] = {
      "exit",
      "status",
      NULL};

  for (int i = 0; builtIns[i] != NULL; i++)
  {
    if (acceptToken(lp, builtIns[i])) 
    {
      Command* newCmd = createCommand(builtIns[i]);
      newCmd->type = CMD_BUILTIN; // Set as built-in command
      printf("new builtIn Cmd: %s\n", newCmd->command);

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
 * The function parseChain parses a chain according to the grammar:
 *
 * <chain>              ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the chain was parsed successfully.
 */
bool parseChain(List *lp, Command** head)
{
  if (parseBuiltIn(lp, head))
  {
    return parseOptions(lp, head);
  }
  if (parsePipeline(lp, head))
  {
    return parseRedirections(lp);
  }
  return false;
}

bool parseInputLineInternal(List* lp, Command** head) {
    if (isEmpty(*lp))
  {
    printf("empty\n");
    return true;
  }

  if (!parseChain(lp, head))
  {
    return false;
  }

  if (acceptToken(lp, "&") || acceptToken(lp, "&&"))
  {
    return parseInputLineInternal(lp, head);
  }
  else if (acceptToken(lp, "||"))
  {
    return parseInputLineInternal(lp, head);
  }
  else if (acceptToken(lp, ";"))
  {
    return parseInputLineInternal(lp, head);
  }

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
bool parseInputLine(List *lp)
{
  Command* head = NULL;

  // Call the internal parsing function with the head pointer
  bool result = parseInputLineInternal(lp, &head);

  printCommands(head);
  
  //free command struct and its options
  return true;
}
