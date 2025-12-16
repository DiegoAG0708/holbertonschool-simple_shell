#include "shell.h"

/**
 * read_line - Read a line from stdin
 *
 * Return: Pointer to string, or NULL on EOF/error
 */
char *read_line(void)
{
char *line = NULL;
size_t cap = 0;
ssize_t n;

n = getline(&line, &cap, stdin);
if (n == -1)
{
free(line);
return (NULL);
}
if (n > 0 && line[n - 1] == '\n')
line[n - 1] = '\0';
return (line);
}
