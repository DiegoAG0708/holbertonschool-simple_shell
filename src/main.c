#include "shell.h"

/**
 * main - Entry point of the shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: Exit status
 */
int main(int argc, char **argv)
{
int status = 0, cmd_idx = 0;
char *line;
char **tokens;

(void)argc;

while (1)
{
if (is_interactive())
print_prompt();

line = read_line();
if (!line)
break;

tokens = tokenize(line);
free(line);

if (!tokens || !tokens[0])
{
free_tokens(tokens);
continue;
}

cmd_idx++;

if (is_builtin(tokens))
status = run_builtin(tokens);
else
status = execute(tokens, argv[0], cmd_idx);

free_tokens(tokens);
}
return (status);
}
