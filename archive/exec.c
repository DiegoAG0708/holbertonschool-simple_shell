#include "shell.h"

/**
 * print_not_found - Print error when command not found
 * @progname: Shell program name
 * @idx: Command index
 * @cmd: Command string
 *
 * Return: void
 */
static void print_not_found(const char *progname, int idx, const char *cmd)
{
char buf[1024];
int n;

n = sprintf(buf, "%s: %d: %s: not found\n", progname, idx, cmd);
write(STDERR_FILENO, buf, n);
}

/**
 * execute - Execute external command
 * @argv: Argument array
 * @progname: Shell program name
 * @cmd_idx: Command index
 *
 * Return: Exit status
 */
int execute(char **argv, const char *progname, int cmd_idx)
{
char *cmd = argv[0];
char *path = NULL;
pid_t pid;
int status;

if (cmd[0] == '/' || (cmd[0] == '.' && (cmd[1] == '/' || cmd[1] == '.')))
path = strdup(cmd);
else
path = find_in_path(cmd);

if (!path)
{
print_not_found(progname, cmd_idx, cmd);
return (127);
}

pid = fork();
if (pid < 0)
{
perror(progname);
free(path);
return (1);
}
if (pid == 0)
{
execve(path, argv, environ);
perror(progname);
_exit(126);
}
else
{
if (wait(&status) == -1)
perror(progname);
free(path);
if (WIFEXITED(status))
return (WEXITSTATUS(status));
return (1);
}
}
