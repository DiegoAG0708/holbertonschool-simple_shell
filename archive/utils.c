#include "shell.h"

/**
 * is_interactive - Check if stdin is a terminal
 *
 * Return: 1 if interactive, 0 otherwise
 */
int is_interactive(void)
{
return (isatty(STDIN_FILENO));
}

/**
 * print_prompt - Print the shell prompt
 *
 * Return: void
 */
void print_prompt(void)
{
write(STDOUT_FILENO, "($) ", 4);
fflush(stdout);
}
