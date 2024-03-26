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

  openPipelines();
  
  while (data->currentCommand) {

    bool shouldExecuteNext = true; 

    if (data->currentCommand->type == CMD_BUILTIN) {
      executeBuiltIns();
    } else {
      executeCommand(envp);
    }

    switch (data->currentCommand->nextOp) {
      case OP_AND: // &&
        shouldExecuteNext = (lastExitStatus == 0);
        break;
      case OP_OR: // ||
        shouldExecuteNext = (lastExitStatus != 0);
        break;
      case OP_SEQ: 
        //TODO: Fix this, should work like /n. remove all the commands before this
        waitProcesses();
        closePipelines(); 
        deleteCommandsUpToCurrent();
        continue; 
      case OP_PIPE: // |
        shouldExecuteNext = true;
      case OP_NONE: // No operator, or end of a command sequence
        data->currentCommand = data->currentCommand->next;
        continue;
    }

    if (!shouldExecuteNext && data->currentCommand->next != NULL) {
      data->currentCommand = data->currentCommand->next->next;
    } else {
      data->currentCommand = data->currentCommand->next;
    }
  }
  waitProcesses();
  closePipelines();
}

/**
 * Executes a command.
 * @param current The command to execute.
 * @param envp The environment variables.
*/
void executeCommand(char **envp) {
  
  data->currentCommand->pid = fork();
  
  if (data->currentCommand->pid  == -1) {
    perror("fork");
    return;
  } else if (data->currentCommand->pid  == 0) {
    signal(SIGQUIT, SIG_DFL); // Reset signal handler to default, do I need this?
    redirectStds();     
    //printf("Finished redirectStds(); Executing command: %s\n", data->currentCommand->command);
    //closeFds();
    //printf("Finished closeFds(); Executing command: %s\n", data->currentCommand->command);
    closePipelines();
    //printf("Finished closePipelines(); Executing command: %s\n", data->currentCommand->command);
    childExecution(); 
    //printf("Finished childExecution(); Executing command: %s\n", data->currentCommand->command);
  } else {
    closeCurrentFds();
  }     
}

void childExecution() {
    Command* cmd = data->currentCommand;
    //printf("Executing child\n");
    //printCommand(cmd);

    if (cmd->type == CMD_BUILTIN) {
        // Execute the built-in command. This assumes you have a function to handle built-in commands directly.
        executeBuiltIns(); // Note: This function should now be adapted to work without arguments, using the global `data`.
    } else {
        // For external commands, build the arguments list including the command itself
        char** args = buildArguments();
        // Execute the command with arguments
        if (execvp(cmd->command, args) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    }
}

void waitProcesses() {
    int status;
    Command* tmp = data->commandList;

    while (tmp) {
        if (tmp->pid >= 0) { 
          waitpid(tmp->pid, &status, 0); 
          updateLastExitStatus(WEXITSTATUS(status)); 
        }
        tmp = tmp->next;
    }
}
