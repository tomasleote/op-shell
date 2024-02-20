#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

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

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtinStr[] = {
  "cd",
  "exit"
};

int numBuiltins() {
  return sizeof(builtinStr) / sizeof(char *);
}

int (*builtinFunc[]) (char **) = {
    &cd,
    &exit
};
    

/*
  Builtin function implementations. Check this later
*/
int cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
  }
  return 1;
}

int exit(char **args) {
  return 0;
}

void parseAndExecute(List *tokenList) {
         
}

/**
 * The function parseExecutable parses an executable.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the executable was parsed successfully.
 */
bool parseExecutable(List *lp) {

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
    int i; 

    if (lp[0]==NULL) {
        return 1;
    }

    //for now, not optimal
    for (i = 0; i < numBuiltins(); i++) {
        if (strcmp(lp[0], builtinStr[i]) == 0) {
            return (builtinFunc[i])(lp);
        }
    }

    return launch(lp);
}

/**
 * The function launch launches a process.
 * @param args arguments to the process (Token list).
 * @return a bool denoting whether the process was launched successfully.
 */
int launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process, here it expects the first argument to be the command
        if (execvp(args[0], args) == -1) {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("shell");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/**
 * Checks whether the input string \param s is an operator.
 * @param s input string.
 * @return a bool denoting whether the current string is an operator.
 */
bool isOperator(char *s) {
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
            NULL
    };

    for (int i = 0; operators[i] != NULL; i++) {
        if (strcmp(s, operators[i]) == 0) return true;
    }
    return false;
}

/**
 * The function parseOptions parses options.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the options were parsed successfully.
 */
bool parseOptions(List *lp) {
    //TODO: store each (*lp)->t as an option, if any exist
    while (*lp != NULL && !isOperator((*lp)->t)) {
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
bool parseCommand(List *lp) {
    return parseExecutable(lp) && parseOptions(lp);
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
bool parsePipeline(List *lp) {
    if (!parseCommand(lp)) {
        return false;
    }

    if (acceptToken(lp, "|")) {
        return parsePipeline(lp);
    }

    return true;
}

/**
 * The function parseFileName parses a filename.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the filename was parsed successfully.
 */
bool parseFileName(List *lp) {
    //TODO: Process the file name appropriately
    char *fileName = (*lp)->t;
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
bool parseRedirections(List *lp) {
    if (isEmpty(*lp)) {
        return true;
    }

    if (acceptToken(lp, "<")) {
        if (!parseFileName(lp)) return false;
        if (acceptToken(lp, ">")) return parseFileName(lp);
        else return true;
    } else if (acceptToken(lp, ">")) {
        if (!parseFileName(lp)) return false;
        if (acceptToken(lp, "<")) return parseFileName(lp);
        else return true;
    }

    return true;
}

/**
 * The function parseBuiltIn parses a builtin.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the builtin was parsed successfully.
 */
bool parseBuiltIn(List *lp) {

    //
    // TODO: Implement the logic for these builtins, and extend with
    // more builtins down the line
    //

    // NULL-terminated array makes it easy to expand this array later
    // without changing the code at other places.
    char *builtIns[] = {
            "exit",
            "status",
            NULL
    };

    for (int i = 0; builtIns[i] != NULL; i++) {
        if (acceptToken(lp, builtIns[i])) return true;
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
bool parseChain(List *lp) {
    if (parseBuiltIn(lp)) {
        return parseOptions(lp);
    }
    if (parsePipeline(lp)) {
        return parseRedirections(lp);
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
bool parseInputLine(List *lp) {
    if (isEmpty(*lp)) {
        return true;
    }

    if (!parseChain(lp)) {
        return false;
    }

    if (acceptToken(lp, "&") || acceptToken(lp, "&&")) {
        return parseInputLine(lp);
    } else if (acceptToken(lp, "||")) {
        return parseInputLine(lp);
    } else if (acceptToken(lp, ";")) {
        return parseInputLine(lp);
    }

    return true;
}
