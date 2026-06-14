#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"
#include "shell.h"
#include <sys/wait.h>
#ifdef __EMSCRIPTEN__
#include "builtins_web.h"
#endif

int lastExitStatus = 0; 

/**
 * Exits the shell
 * @param args List of arguments.
*/
int exitShell(char **args) {
  printf("Exiting shell!\n");
  exit(0);
}

/**
 * Prints the most recent exit status.
 * @param args List of arguments.
*/
int statusShell(char **args) {
    printf("The most recent exit code is: %d\n", lastExitStatus);
    return 1; // Indicate success, shell continues running
}

/**
 * Changes the working directory. Must be a built-in: a forked child cannot
 * change the parent shell's cwd. Defaults to $HOME when no argument is given.
 * @param args option list (args[0] is the target directory, if any).
 */
int cdShell(char **args) {
  const char *dir = (args && args[0]) ? args[0] : getenv("HOME");
  if (dir == NULL) {
    dir = "/";
  }
  if (chdir(dir) != 0) {
    fprintf(stderr, "cd: %s: No such file or directory\n", dir);
    lastExitStatus = 1;
    return 1;
  }
  lastExitStatus = 0;
  return 0;
}

/**
 * Prints the current working directory.
 * @param args unused.
 */
int pwdShell(char **args) {
  (void)args;
  char buf[1024];
  if (getcwd(buf, sizeof buf) != NULL) {
    printf("%s\n", buf);
    lastExitStatus = 0;
    return 0;
  }
  perror("pwd");
  lastExitStatus = 1;
  return 1;
}

/**
 * Lists the built-in commands. External commands run via $PATH.
 * @param args unused.
 */
int helpShell(char **args) {
  (void)args;
  printf(
      "Built-in commands:\n"
      "  cd [dir]   change directory (defaults to $HOME)\n"
      "  pwd        print working directory\n"
      "  status     exit code of the last command\n"
      "  help       this help\n"
      "  exit       quit the shell\n"
      "Any other command runs as an external program found in $PATH.\n");
  lastExitStatus = 0;
  return 0;
}


/**
 * Executes the linked list.
 * @param head pointer to current element to execute.
 * @param envp The environment variables.
*/
void execute(Command* head, char **envp) {
  // lastExitStatus = 0;
#ifdef __EMSCRIPTEN__
  (void)envp;
#endif
  Command* current = head;
  while (current) {
#ifdef __EMSCRIPTEN__
      executeWeb(current);
#else
      if (current->type == CMD_BUILTIN) {
        //printf("Executing built-in %s\n", current->command);
        executeBuiltIns(current);
      } else {
        //printf("Executing command %s\n", current->command);
        executeCommand(current, envp);
      }
#endif
    current = current->next;
    }
}

/**
 * Executes built-in commands.
 * @param args List of arguments.
*/
void executeBuiltIns(Command* current) {
  
  if (strcmp(current->command, "exit") == 0) {
        exitShell(current->options);
  } else if (strcmp(current->command, "status") == 0) {
        statusShell(current->options);
  } else if (strcmp(current->command, "cd") == 0) {
        cdShell(current->options);
  } else if (strcmp(current->command, "pwd") == 0) {
        pwdShell(current->options);
  } else if (strcmp(current->command, "help") == 0) {
        helpShell(current->options);
  } else {
        printf("Unknown command: %s\n", current->command);
  }
}

/**
 * Executes a command.
 * @param current The command to execute.
 * @param envp The environment variables.
*/
#ifndef __EMSCRIPTEN__
void executeCommand(Command* current, char **envp) {

  pid_t pid = fork();
  
  if (pid == -1) {
    perror("fork");
    return;
  }
  
  if (pid == 0) {
    // Build argv locally (argv[0] = program name) instead of mutating the
    // Command, which would alias command into options[0] and leak the old array.
    char **argv = malloc((current->optionCount + 2) * sizeof(char*));
    if (argv == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    argv[0] = current->command;
    for (int i = 0; i < current->optionCount; i++) {
      argv[i + 1] = current->options[i];
    }
    argv[current->optionCount + 1] = NULL;
    if (execvp(current->command, argv) == -1) {
      perror("Error: command not found!");
      free(argv);
      exit(EXIT_FAILURE);
    }
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0); // Wait for the command to complete
    
    // if (WIFEXITED(status)) {
        lastExitStatus = WEXITSTATUS(status); // Update global status
    // }
  }
      
}
#endif
