#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>

extern char **environ;

/* main loop */
char *read_line(void);

/* prompt */
int is_interactive(void);
void print_prompt(void);

/* parsing */
char **tokenize(const char *line);
void free_tokens(char **argv);

/* builtins */
int is_builtin(char **argv);
int run_builtin(char **argv);

/* execution */
int execute(char **argv, const char *progname, int cmd_idx);

/* path resolution */
char *find_in_path(const char *cmd);

#endif /* SHELL_H */
