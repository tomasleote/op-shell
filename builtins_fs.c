#ifdef __EMSCRIPTEN__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "scanner.h"
#include "parser.h"
#include "command.h"
#include "builtins_web.h"

void cmd_ls(Command *cmd) {
    const char *path = cmd->optionCount > 0 ? cmd->options[0] : ".";
    DIR *d = opendir(path);
    if (!d) {
        sh_out("ls: %s: no such directory\n", path);
        lastExitStatus = 1;
        return;
    }
    struct dirent *e;
    int count = 0;
    while ((e = readdir(d)) != NULL) {
        if (e->d_name[0] == '.') continue; /* hide dotfiles (no -a flag) */
        char full[1024];
        snprintf(full, sizeof full, "%s/%s", path, e->d_name);
        struct stat st;
        int isdir = (stat(full, &st) == 0 && S_ISDIR(st.st_mode));
        sh_out("%s%s%s", count ? "  " : "", e->d_name, isdir ? "/" : "");
        count++;
    }
    if (count) sh_out("\n");
    closedir(d);
    lastExitStatus = 0;
}

void cmd_cat(Command *cmd) {
    if (cmd->optionCount < 1) {
        sh_out("cat: missing file operand\n");
        lastExitStatus = 1;
        return;
    }
    for (int i = 0; i < cmd->optionCount; i++) {
        FILE *f = fopen(cmd->options[i], "r");
        if (!f) {
            sh_out("cat: %s: no such file\n", cmd->options[i]);
            lastExitStatus = 1;
            continue;
        }
        char buf[4096];
        size_t n;
        while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
            out_append(buf, n);
        }
        fclose(f);
        lastExitStatus = 0;
    }
}

void cmd_mkdir(Command *cmd) {
    if (cmd->optionCount < 1) {
        sh_out("mkdir: missing directory operand\n");
        lastExitStatus = 1;
        return;
    }
    if (mkdir(cmd->options[0], 0755) != 0) {
        sh_out("mkdir: cannot create '%s'\n", cmd->options[0]);
        lastExitStatus = 1;
    } else {
        lastExitStatus = 0;
    }
}

void cmd_touch(Command *cmd) {
    if (cmd->optionCount < 1) {
        sh_out("touch: missing file operand\n");
        lastExitStatus = 1;
        return;
    }
    FILE *f = fopen(cmd->options[0], "a");
    if (!f) {
        sh_out("touch: cannot create '%s'\n", cmd->options[0]);
        lastExitStatus = 1;
        return;
    }
    fclose(f);
    lastExitStatus = 0;
}

static int remove_recursive(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    if (S_ISDIR(st.st_mode)) {
        DIR *d = opendir(path);
        if (!d) return -1;
        struct dirent *e;
        while ((e = readdir(d)) != NULL) {
            if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
            char full[1024];
            snprintf(full, sizeof full, "%s/%s", path, e->d_name);
            remove_recursive(full);
        }
        closedir(d);
        return rmdir(path);
    }
    return unlink(path);
}

void cmd_rm(Command *cmd) {
    int recursive = 0;
    int start = 0;
    if (cmd->optionCount > 0 && strcmp(cmd->options[0], "-r") == 0) {
        recursive = 1;
        start = 1;
    }
    if (cmd->optionCount <= start) {
        sh_out("rm: missing operand\n");
        lastExitStatus = 1;
        return;
    }
    for (int i = start; i < cmd->optionCount; i++) {
        const char *p = cmd->options[i];
        struct stat st;
        if (stat(p, &st) != 0) {
            sh_out("rm: %s: no such file or directory\n", p);
            lastExitStatus = 1;
            continue;
        }
        if (S_ISDIR(st.st_mode) && !recursive) {
            sh_out("rm: %s: is a directory (use -r)\n", p);
            lastExitStatus = 1;
            continue;
        }
        if ((recursive ? remove_recursive(p) : unlink(p)) != 0) {
            sh_out("rm: cannot remove '%s'\n", p);
            lastExitStatus = 1;
        } else {
            lastExitStatus = 0;
        }
    }
}

/* Shows how the real scanner + parser read a line: the token list, then the
   parsed command list. Reuses the genuine getTokenList()/parseInputLine(). */
void cmd_parse(Command *cmd) {
    if (cmd->optionCount < 1) {
        sh_out("parse: provide a line, e.g.: parse ls -la\n");
        lastExitStatus = 1;
        return;
    }
    size_t len = 0;
    for (int i = 0; i < cmd->optionCount; i++) len += strlen(cmd->options[i]) + 1;
    char *line = malloc(len + 1);
    if (!line) return;
    line[0] = '\0';
    for (int i = 0; i < cmd->optionCount; i++) {
        if (i) strcat(line, " ");
        strcat(line, cmd->options[i]);
    }

    List tokens = getTokenList(line);
    sh_out("tokens: ");
    for (List t = tokens; t != NULL; t = t->next) {
        sh_out("[%s]%s", t->t, t->next ? " " : "");
    }
    sh_out("\n");

    List cursor = tokens;
    int ok = 0;
    Command *parsed = parseInputLine(&cursor, &ok);
    for (Command *p = parsed; p != NULL; p = p->next) {
        sh_out("cmd: %-8s type: %s  options:", p->command,
               p->type == CMD_BUILTIN ? "builtin " : "external");
        for (int i = 0; i < p->optionCount; i++) sh_out(" %s", p->options[i]);
        sh_out("\n");
    }

    while (parsed) {
        Command *next = parsed->next;
        freeCommand(parsed);
        parsed = next;
    }
    freeTokenList(tokens);
    free(line);
    lastExitStatus = 0;
}

#endif /* __EMSCRIPTEN__ */
