#include "shell.h"

/**
 * is_builtin - Check if command is builtin
 * @argv: Argument array
 *
 * Return: 1 if builtin, 0 otherwise
 */
int is_builtin(char **argv)
{
if (!argv || !argv[0])
return (0);
return (!strcmp(argv[0], "exit") || !strcmp(argv[0], "env"));
}

/**
 * run_builtin - Execute builtin command
 * @argv: Argument array
 *
 * Return: Status code
 */
int run_builtin(char **argv)
{
if (!strcmp(argv[0], "exit"))
{
int status = 0;

if (argv[1])
status = atoi(argv[1]);
exit(status);
}
if (!strcmp(argv[0], "env"))
{
char **p = environ;

while (p && *p)
{
write(STDOUT_FILENO, *p, strlen(*p));
write(STDOUT_FILENO, "\n", 1);
p++;
}
return (0);
}
return (0);
}
