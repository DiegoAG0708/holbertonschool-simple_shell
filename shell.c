#include "shell.h"

/**
 * trim_spaces - Removes leading and trailing spaces and tabs
 * @str: Input string
 *
 * Return: Trimmed string
 */
char *trim_spaces(char *str)
{
char *end;

while (*str == ' ' || *str == '\t')
str++;

if (*str == '\0')
return (str);

end = str + strlen(str) - 1;
while (end > str && (*end == ' ' || *end == '\t'))
{
*end = '\0';
end--;
}
return (str);
}

/**
 * tokenize_line - Splits a line into tokens separated by space or tab
 * @line: Input string (modified in place)
 *
 * Return: Array of tokens (NULL-terminated), malloc'd; caller frees
 */
char **tokenize_line(char *line)
{
char **argv;
int cap = 64, argc = 0;
char *p = line, *start;

argv = malloc(sizeof(char *) * cap);
if (!argv)
return (NULL);

while (*p)
{
while (*p == ' ' || *p == '\t')
p++;
if (*p == '\0')
break;

start = p;
while (*p && *p != ' ' && *p != '\t')
p++;
if (*p)
{
*p = '\0';
p++;
}

argv[argc++] = start;
if (argc + 1 >= cap)
break;
}
argv[argc] = NULL;
return (argv);
}

/**
 * get_env_value - Get environment variable value without using getenv
 * @name: Variable name
 *
 * Return: Pointer to value string or NULL
 */
char *get_env_value(const char *name)
{
size_t nlen;
int i;

if (!name)
return (NULL);
nlen = strlen(name);

for (i = 0; environ && environ[i]; i++)
{
if (!strncmp(environ[i], name, nlen) && environ[i][nlen] == '=')
return (environ[i] + nlen + 1);
}
return (NULL);
}

/**
 * join_path - Build "dir/cmd" path safely
 * @dir: Directory string
 * @cmd: Command string
 *
 * Return: Newly allocated path string or NULL
 */
static char *join_path(const char *dir, const char *cmd)
{
size_t ld, lc;
char *out;

ld = strlen(dir);
lc = strlen(cmd);
out = malloc(ld + 1 + lc + 1);
if (!out)
return (NULL);
memcpy(out, dir, ld);
out[ld] = '/';
memcpy(out + ld + 1, cmd, lc);
out[ld + 1 + lc] = '\0';
return (out);
}

/**
 * int_to_str - Convert positive integer to string
 * @n: Integer
 * @buf: Buffer to write into (at least 12 bytes)
 *
 * Return: Pointer to buf containing the number
 */
static char *int_to_str(int n, char *buf)
{
int i = 0, j;
char tmp[12];

if (n == 0)
{
buf[0] = '0';
buf[1] = '\0';
return (buf);
}
while (n > 0 && i < 11)
{
tmp[i++] = (char)('0' + (n % 10));
n /= 10;
}
for (j = 0; j < i; j++)
buf[j] = tmp[i - j - 1];
buf[i] = '\0';
return (buf);
}

/**
 * write_not_found - Write "prog: line: cmd: not found" to stderr
 * @prog: Program name (argv[0])
 * @line_no: Line number (1-based)
 * @cmd: Command name
 */
static void write_not_found(const char *prog, int line_no, const char *cmd)
{
char num[12];
char buf[512];
size_t pos = 0, len;

/* prog */
if (prog)
{
len = strlen(prog);
if (pos + len < sizeof(buf))
{
memcpy(buf + pos, prog, len);
pos += len;
}
}
/* ": " */
if (pos + 2 < sizeof(buf))
{
buf[pos++] = ':';
buf[pos++] = ' ';
}
/* line number */
int_to_str(line_no, num);
len = strlen(num);
if (pos + len < sizeof(buf))
{
memcpy(buf + pos, num, len);
pos += len;
}
/* ": " */
if (pos + 2 < sizeof(buf))
{
buf[pos++] = ':';
buf[pos++] = ' ';
}
/* cmd */
if (cmd)
{
len = strlen(cmd);
if (pos + len < sizeof(buf))
{
memcpy(buf + pos, cmd, len);
pos += len;
}
}
/* ": not found\n" */
if (pos + 13 < sizeof(buf))
{
memcpy(buf + pos, ": not found\n", 12);
pos += 12;
}
/* write to stderr */
if (pos > 0)
write(STDERR_FILENO, buf, pos);
}

/**
 * resolve_command - Resolve command via PATH or direct path
 * @cmd: Command name
 *
 * Return: Newly allocated full path or NULL
 */
char *resolve_command(const char *cmd)
{
const char *path_env;
char *copy, *p, *start, *candidate;
struct stat st;

if (!cmd || *cmd == '\0')
return (NULL);

if (strchr(cmd, '/'))
{
if (stat(cmd, &st) == 0 && access(cmd, X_OK) == 0)
{
char *ret = malloc(strlen(cmd) + 1);
if (ret)
strcpy(ret, cmd);
return (ret);
}
return (NULL);
}

path_env = get_env_value("PATH");
if (!path_env || *path_env == '\0')
return (NULL);

copy = malloc(strlen(path_env) + 1);
if (!copy)
return (NULL);
strcpy(copy, path_env);

p = copy;
while (*p)
{
start = p;
while (*p && *p != ':')
p++;
if (*p == ':')
*p++ = '\0';

if (*start == '\0')
start = ".";

candidate = join_path(start, cmd);
if (candidate)
{
if (stat(candidate, &st) == 0 && access(candidate, X_OK) == 0)
{
free(copy);
return (candidate);
}
free(candidate);
}
}
free(copy);
return (NULL);
}

/**
 * main - Simple shell 0.3 with PATH support and proper exit status
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: Exit status (last command), 127 if not found
 */
int main(int argc, char **argv)
{
char *line = NULL, *cmd;
size_t n = 0;
ssize_t read;
char **args;
char *path;
pid_t pid;
int status = 0;
int line_no = 1;

(void)argc;

for (;;)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "#cisfun$ ", 9);

read = getline(&line, &n, stdin);
if (read == -1)
{
free(line);
return (status);
}

cmd = strtok(line, "\n");
while (cmd)
{
cmd = trim_spaces(cmd);
if (*cmd == '\0')
{
cmd = strtok(NULL, "\n");
line_no++;
continue;
}

args = tokenize_line(cmd);
if (!args || !args[0] || args[0][0] == '\0')
{
if (args)
free(args);
cmd = strtok(NULL, "\n");
line_no++;
continue;
}

path = resolve_command(args[0]);
if (!path)
{
/* report not found and set status to 127; do not fork */
write_not_found(argv[0] ? argv[0] : "./hsh", line_no, args[0]);
status = 127;
free(args);
cmd = strtok(NULL, "\n");
line_no++;
continue;
}

pid = fork();
if (pid == -1)
{
/* fork failed; keep quiet, set non-zero status */
status = 1;
free(args);
free(path);
cmd = strtok(NULL, "\n");
line_no++;
continue;
}
if (pid == 0)
{
execve(path, args, environ);
/* exec failed in child: exit with 126 (common for "cannot execute") */
_exit(126);
}
if (waitpid(pid, &status, 0) == -1)
status = 1;
/* normalize to exit code */
if (WIFEXITED(status))
status = WEXITSTATUS(status);
else
status = 1;

free(args);
free(path);

cmd = strtok(NULL, "\n");
line_no++;
}
}
free(line);
return (status);
}
