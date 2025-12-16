#include "shell.h"

/**
 * tokenize - Split line into tokens
 * @line: Input string
 *
 * Return: Array of tokens, NULL on failure
 */
char **tokenize(const char *line)
{
char *copy, *tok;
size_t cap = 8, i = 0;
char **argv = malloc(sizeof(char *) * cap);

if (!argv)
return (NULL);

copy = strdup(line);
if (!copy)
{
free(argv);
return (NULL);
}

tok = strtok(copy, " \t");
while (tok)
{
if (i + 1 >= cap)
{
cap *= 2;
argv = realloc(argv, sizeof(char *) * cap);
if (!argv)
{
free(copy);
return (NULL);
}
}
argv[i++] = strdup(tok);
tok = strtok(NULL, " \t");
}
argv[i] = NULL;
free(copy);
return (argv);
}

/**
 * free_tokens - Free token array
 * @argv: Array to free
 *
 * Return: void
 */
void free_tokens(char **argv)
{
size_t i;

if (!argv)
return;
for (i = 0; argv[i]; i++)
free(argv[i]);
free(argv);
}
