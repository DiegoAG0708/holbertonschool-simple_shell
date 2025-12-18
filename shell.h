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
char **tokenize(char *line);
char *find_in_path(char *cmd);

#endif /* SHELL_H */
