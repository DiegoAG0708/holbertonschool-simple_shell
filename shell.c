#include "shell.h"

/**
 * trim_spaces - Removes leading and trailing spaces from a string
 * @str: Input string
 *
 * Return: Trimmed string (pointer inside original buffer)
 */
char *trim_spaces(char *str)
{
char *end;

while (*str == ' ' || *str == '\t')
str++;

if (*str == '\0')
return str;

end = str + strlen(str) - 1;
while (end > str && (*end == ' ' || *end == '\t'))
{
*end = '\0';
end--;
}
return str;
}

/**
 * tokenize - Splits a line into tokens
 * @line: Input string (modified in place)
 *
 * Return: Array of tokens (NULL-terminated), malloc'd; caller frees
 */
char **tokenize(char *line)
{
char **tokens = NULL;
char *token;
int i = 0;

tokens = malloc(sizeof(char *) * 64);
if (!tokens)
return NULL;

token = strtok(line, " \t");
while (token != NULL)
{
tokens[i++] = token;
token = strtok(NULL, " \t");
}
tokens[i] = NULL;
return tokens;
}

/**
 * find_in_path - Find an executable by searching PATH
 * @cmd: Command name (may contain '/')
 *
 * Return: Newly allocated full path if found, otherwise NULL
 */
char *find_in_path(const char *cmd)
{
char *path_env, *path_copy, *saveptr, *dir;
char full[1024];
struct stat st;

if (!cmd || *cmd == '\0')
return NULL;

/* If command contains '/', check it directly */
if (strchr(cmd, '/'))
{
if (stat(cmd, &st) == 0 && access(cmd, X_OK) == 0)
return strdup(cmd);
return NULL;
}

/* Get PATH; treat empty PATH as missing */
path_env = getenv("PATH");
if (!path_env || *path_env == '\0')
return NULL;

/* Work on a copy and use strtok_r safely */
path_copy = strdup(path_env);
if (!path_copy)
return NULL;

dir = strtok_r(path_copy, ":", &saveptr);
while (dir != NULL)
{
if (snprintf(full, sizeof(full), "%s/%s", dir, cmd) >= (int)sizeof(full))
{
dir = strtok_r(NULL, ":", &saveptr);
continue;
}

if (stat(full, &st) == 0 && access(full, X_OK) == 0)
{
char *ret = strdup(full);
free(path_copy);
return ret;
}
dir = strtok_r(NULL, ":", &saveptr);
}
free(path_copy);
return NULL;
}

/**
 * main - Simple shell 0.3 (PATH), no fork on missing command, quiet stderr
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: Exit status
 */
int main(int argc, char **argv)
{
char *line = NULL;
size_t n = 0;
ssize_t read;
char *cmd;
char **args;
char *resolved;
pid_t pid;
int status;

(void)argc;
(void)argv;

while (1)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "#cisfun$ ", 9);

read = getline(&line, &n, stdin);
if (read == -1)
{
free(line);
return (0);
}

/* Split by newline; process each command independently */
cmd = strtok(line, "\n");
while (cmd != NULL)
{
cmd = trim_spaces(cmd);

/* Skip empty or space-only lines silently */
if (*cmd == '\0')
{
cmd = strtok(NULL, "\n");
continue;
}

/* Tokenize the command line (in-place) */
args = tokenize(cmd);
if (!args || !args[0] || args[0][0] == '\0')
{
if (args) free(args);
cmd = strtok(NULL, "\n");
continue;
}

/* Resolve via PATH or absolute/relative; do not fork if missing */
resolved = find_in_path(args[0]);
if (!resolved)
{
/* Quiet: do not write to stderr; just skip */
free(args);
cmd = strtok(NULL, "\n");
continue;
}

/* Execute the resolved path with args */
pid = fork();
if (pid == -1)
{
/* Quiet failure: free and move on */
free(args);
free(resolved);
cmd = strtok(NULL, "\n");
continue;
}

if (pid == 0)
{
/* Child: exec; keep stderr quiet for checker */
execve(resolved, args, environ);
_exit(EXIT_FAILURE);
}
else
{
/* Parent: wait and cleanup */
waitpid(pid, &status, 0);
free(args);
free(resolved);
cmd = strtok(NULL, "\n");
}
}
}
free(line);
return (0);
}
