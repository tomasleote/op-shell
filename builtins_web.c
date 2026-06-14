#ifdef __EMSCRIPTEN__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include "scanner.h"
#include "parser.h"
#include "command.h"
#include "shell.h"
#include "builtins_web.h"

/* Growable output buffer for the current execute_line() call. A single buffer
   is reused across calls (valid until the next call), so there is nothing to
   free across the JS boundary. */
static char *g_out = NULL;
static size_t g_len = 0;
static size_t g_cap = 0;

void out_append(const char *data, size_t n) {
    if (g_len + n + 1 > g_cap) {
        size_t cap = g_cap ? g_cap : 64;
        while (g_len + n + 1 > cap) cap *= 2;
        char *nb = realloc(g_out, cap);
        if (!nb) return;
        g_out = nb;
        g_cap = cap;
    }
    memcpy(g_out + g_len, data, n);
    g_len += n;
    g_out[g_len] = '\0';
}

void sh_out(const char *fmt, ...) {
    char stackbuf[1024];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(stackbuf, sizeof stackbuf, fmt, ap);
    va_end(ap);
    if (n < 0) return;
    if ((size_t)n < sizeof stackbuf) {
        out_append(stackbuf, (size_t)n);
        return;
    }
    char *big = malloc((size_t)n + 1);
    if (!big) return;
    va_start(ap, fmt);
    vsnprintf(big, (size_t)n + 1, fmt, ap);
    va_end(ap);
    out_append(big, (size_t)n);
    free(big);
}

static void out_reset(void) {
    g_len = 0;
    if (!g_out) {
        g_out = malloc(64);
        if (g_out) g_cap = 64;
    }
    if (g_out) g_out[0] = '\0';
}

static void freeCommandList(Command *head) {
    while (head) {
        Command *next = head->next;
        freeCommand(head);
        head = next;
    }
}

static void cmd_help(void) {
    sh_out(
        "Available commands:\n"
        "  help            this help\n"
        "  ls [dir]        list a directory\n"
        "  cd [dir]        change directory\n"
        "  pwd             current directory\n"
        "  cat <file>      print a file\n"
        "  echo [text]     print text\n"
        "  mkdir <dir>     create a directory\n"
        "  touch <file>    create an empty file\n"
        "  rm [-r] <path>  remove a file or directory\n"
        "  clear           clear the screen\n"
        "  parse <line>    show how the scanner/parser read a line\n"
        "  status          exit code of the last command\n"
        "  exit            end the session\n");
    lastExitStatus = 0;
}

static void cmd_pwd(void) {
    char buf[1024];
    if (getcwd(buf, sizeof buf)) {
        sh_out("%s\n", buf);
        lastExitStatus = 0;
    } else {
        sh_out("pwd: cannot get the current directory\n");
        lastExitStatus = 1;
    }
}

static void cmd_cd(Command *cmd) {
    const char *dir = cmd->optionCount > 0 ? cmd->options[0] : "/home/guest";
    if (chdir(dir) != 0) {
        sh_out("cd: %s: no such directory\n", dir);
        lastExitStatus = 1;
    } else {
        lastExitStatus = 0;
    }
}

static void cmd_echo(Command *cmd) {
    for (int i = 0; i < cmd->optionCount; i++) {
        sh_out("%s%s", i ? " " : "", cmd->options[i]);
    }
    sh_out("\n");
    lastExitStatus = 0;
}

static void cmd_clear(void) {
    out_append("\f", 1); /* Section 5 clears the screen when it sees form-feed. */
    lastExitStatus = 0;
}

static void cmd_exit(void) {
    /* Calling exit() would tear down the wasm module; just say goodbye. */
    sh_out("Session ended. Reload the page to start over.\n");
    lastExitStatus = 0;
}

static void cmd_status(void) {
    sh_out("The most recent exit code is: %d\n", lastExitStatus);
}

void executeWeb(Command *cmd) {
    const char *c = cmd->command;
    if (!c) return;
    if      (strcmp(c, "help")   == 0) cmd_help();
    else if (strcmp(c, "pwd")    == 0) cmd_pwd();
    else if (strcmp(c, "cd")     == 0) cmd_cd(cmd);
    else if (strcmp(c, "echo")   == 0) cmd_echo(cmd);
    else if (strcmp(c, "clear")  == 0) cmd_clear();
    else if (strcmp(c, "exit")   == 0) cmd_exit();
    else if (strcmp(c, "status") == 0) cmd_status();
    else if (strcmp(c, "ls")     == 0) cmd_ls(cmd);
    else if (strcmp(c, "cat")    == 0) cmd_cat(cmd);
    else if (strcmp(c, "mkdir")  == 0) cmd_mkdir(cmd);
    else if (strcmp(c, "touch")  == 0) cmd_touch(cmd);
    else if (strcmp(c, "rm")     == 0) cmd_rm(cmd);
    else if (strcmp(c, "parse")  == 0) cmd_parse(cmd);
    else {
        sh_out("Unknown command: %s\n", c);
        lastExitStatus = 127;
    }
}

char *execute_line(const char *line) {
    out_reset();
    if (!line) return g_out ? g_out : (char *)"";

    char *input = strdup(line);
    if (!input) return g_out ? g_out : (char *)"";

    List tokens = getTokenList(input);
    List cursor = tokens; /* parseInputLine advances cursor; tokens keeps the head to free */
    int parsedSuccessfully = 0;
    Command *cmds = parseInputLine(&cursor, &parsedSuccessfully);

    if (cursor == NULL && parsedSuccessfully) {
        execute(cmds, NULL);
    } else {
        sh_out("Error: invalid syntax!\n");
    }

    freeCommandList(cmds);
    freeTokenList(tokens);
    free(input);
    return g_out ? g_out : (char *)"";
}

#endif /* __EMSCRIPTEN__ */
