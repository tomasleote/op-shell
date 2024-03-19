#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shellComponents.h"
#include "parsingTools.h"

int lastExitStatus = 0;

void updateLastExitStatus (int status) {
  lastExitStatus = status;
}

/**
 * Executes the linked list.
 * @param head pointer to current element to execute.
 * @param envp The environment variables.
*/
void execute(char **envp) {
  data->currentCommand = data->commandList;
  
  while (data->currentCommand) {

    bool shouldExecuteNext = true; 

    if (data->currentCommand->type == CMD_BUILTIN) {
      //printf("Executing built-in %s\n", current->command);
      executeBuiltIns();
    } else {
      //printf("Executing command %s\n", current->command);
      executeCommand(envp);
    }

    switch (data->currentCommand->nextOp) {
      case OP_AND: // &&
        shouldExecuteNext = (lastExitStatus == 0);
        break;
      case OP_OR: // ||
        shouldExecuteNext = (lastExitStatus != 0);
        break;
      case OP_SEQ: // ; and \n (handled the same way)
      case OP_PIPE: // |
      case OP_NONE: // No operator, or end of a command sequence
        shouldExecuteNext = true; // Always execute the next command
        break;
    }

    // Decision to skip the next command or continue
    if (!shouldExecuteNext && data->currentCommand->next != NULL) {
      // Skip next command if conditions dictate
      data->currentCommand = data->currentCommand->next->next;
    } else {
      // Proceed to the next command normally
      data->currentCommand = data->currentCommand->next;
    }
  }
}

/**
 * Executes a command.
 * @param current The command to execute.
 * @param envp The environment variables.
*/
void executeCommand(char **envp) {
  
  pid_t pid = fork();
  
  if (pid == -1) {
    perror("fork");
    return;
  }
  
  if (pid == 0) {
    addCommandToOptions();
    if (execvp(data->currentCommand->command, data->currentCommand->options) == -1) {
      printf("Error: command not found!\n");
      // Free all data!
      exit(127); //errno
    }
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0); // Wait for the command to complete
    updateLastExitStatus(WEXITSTATUS(status));
  }
      
}

/**
 * Adds a command to the options array.
 * @param current The command to add.
 * @return void
*/
void addCommandToOptions () {
  // Create an array for execvp arguments
  char **args = malloc((data->currentCommand->optionCount + 2) * sizeof(char*)); // +2 for command and NULL terminator
  args[0] = data->currentCommand->command; // Set command as first argument
  for (int i = 0; i < data->currentCommand->optionCount; i++) {
    args[i + 1] = data->currentCommand->options[i]; // Copy options
  }
  args[data->currentCommand->optionCount + 1] = NULL; // NULL-terminate the array
  data->currentCommand->options = args;
}
