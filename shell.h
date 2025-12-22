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

char *trim_spaces(char *str);
char **tokenize_line(char *line);
char *get_env_value(const char *name);
char *resolve_command(const char *cmd);

#endif /* SHELL_H */
