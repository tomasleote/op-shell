#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"


int exitShell(char **args) {
  printf("Exiting shell!\n");
  exit(0);
}

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtinStr[] = {
    "exit"};

int numBuiltins() {
  return sizeof(builtinStr) / sizeof(char *);
}

int (*builtinFunc[])(char **) = {
    &exitShell
};

/**
 * Executes the linked list.
 * @param head pointer to current element to execute.
 * @param envp The environment variables.
*/
void execute(Command* head, char **envp) {
  Command* current = head;
  while (current) {
      if (current->type == CMD_BUILTIN) {
        executeBuiltIns(current->options);
      } else {
        executeCommand(current, envp);
      }
    current = current->next;
    }
}

/**
 * Executes built-in commands.
 * @param args List of arguments.
*/
void executeBuiltIns(char **args) {
  for (int i = 0; i < numBuiltins(); i++) {
    if (strcmp(args[0], builtinStr[i]) == 0) {
      (*builtinFunc[i])(args);
      return;
    }
  }
}

/**
 * Executes a command.
 * @param current The command to execute.
 * @param envp The environment variables.
*/
void executeCommand( Command* current, char **envp) {
  pid_t pid = fork();
  
  if (pid == -1) {
    perror("fork");
    return;
  }
  
  if (pid == 0) {
    //printf("cmd: %s, cmd->path: %s\n", current->command, current->commandPath);
    //printCommands(current);
    if (execve(current->commandPath, current->options, envp) == -1) {
      perror("execvp");
      exit(EXIT_FAILURE);
      //clean data here
    }
  } else if (pid > 0) {
          // Parent process
          wait(NULL);
        }
}

