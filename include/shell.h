#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

/* Global environment variable */
extern char **environ;

/* Function prototypes */

/* Read a line from stdin */
char *read_line(void);

/* Split a line into tokens */
char **tokenize(const char *line);

/* Free memory allocated for tokens */
void free_tokens(char **argv);

/* Check if command is a builtin */
int is_builtin(char **argv);

/* Run builtin commands */
int run_builtin(char **argv);

/* Find executable in PATH */
char *find_in_path(const char *cmd);

/* Execute external command */
int execute(char **argv, const char *progname, int cmd_idx);

/* Prompt helpers */
int is_interactive(void);
void print_prompt(void);

#endif /* SHELL_H */
