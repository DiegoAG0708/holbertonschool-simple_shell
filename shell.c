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

while (*str == ' ')
str++;

if (*str == '\0')
return str;

end = str + strlen(str) - 1;
while (end > str && *end == ' ')
{
*end = '\0';
end--;
}

return str;
}

/**
 * tokenize - Splits a line into tokens
 * @line: Input string
 *
 * Return: Array of tokens (NULL-terminated)
 */
char **tokenize(char *line)
{
char **tokens = NULL;
char *token;
int i = 0;

tokens = malloc(sizeof(char *) * 64);
if (!tokens)
return NULL;

token = strtok(line, " ");
while (token != NULL)
{
tokens[i++] = token;
token = strtok(NULL, " ");
}
tokens[i] = NULL;
return tokens;
}

/**
 * main - Simple shell 0.1 with argument support
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
pid_t pid;
int status;

(void)argc;

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

cmd = strtok(line, "\n");
while (cmd != NULL)
{
cmd = trim_spaces(cmd);

if (*cmd == '\0')
{
cmd = strtok(NULL, "\n");
continue;
}

args = tokenize(cmd);
if (!args)
{
cmd = strtok(NULL, "\n");
continue;
}

pid = fork();
if (pid == -1)
{
perror(argv[0]);
cmd = strtok(NULL, "\n");
continue;
}
if (pid == 0)
{
if (execve(args[0], args, environ) == -1)
{
perror(argv[0]);
}
exit(EXIT_FAILURE);
}
else
{
waitpid(pid, &status, 0);
cmd = strtok(NULL, "\n");
}
}
}
free(line);
return (0);
}
