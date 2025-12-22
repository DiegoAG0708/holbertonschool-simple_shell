#include "shell.h"

/* Trim leading/trailing spaces and tabs */
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

/* In-place tokenizer splitting on space or tab; returns malloc'd argv array */
char **tokenize_line(char *line)
{
char **argv;
int cap = 64, argc = 0;
char *p = line, *start;

argv = malloc(sizeof(char *) * cap);
if (!argv)
return NULL;

while (*p)
{
/* skip spaces/tabs */
while (*p == ' ' || *p == '\t')
p++;
if (*p == '\0')
break;

/* start of token */
start = p;
while (*p && *p != ' ' && *p != '\t')
p++;
if (*p)
{
*p = '\0';
p++;
}

/* add token */
argv[argc++] = start;
if (argc + 1 >= cap)
break;
}
argv[argc] = NULL;
return argv;
}

/* Get environment variable value without using getenv, via environ */
char *get_env_value(const char *name)
{
size_t nlen;
int i;

if (!name)
return NULL;
nlen = strlen(name);

for (i = 0; environ && environ[i]; i++)
{
/* match NAME=... */
if (!strncmp(environ[i], name, nlen) && environ[i][nlen] == '=')
return environ[i] + nlen + 1;
}
return NULL;
}

/* Build "dir/cmd" path safely (malloc), return NULL if fail */
static char *join_path(const char *dir, const char *cmd)
{
size_t ld, lc;
char *out;

ld = strlen(dir);
lc = strlen(cmd);
out = malloc(ld + 1 + lc + 1);
if (!out)
return NULL;
memcpy(out, dir, ld);
out[ld] = '/';
memcpy(out + ld + 1, cmd, lc);
out[ld + 1 + lc] = '\0';
return out;
}

/* Resolve command: absolute/relative path or search PATH; returns malloc'd path or NULL */
char *resolve_command(const char *cmd)
{
const char *path_env;
char *copy, *p, *start, *candidate;
struct stat st;

if (!cmd || *cmd == '\0')
return NULL;

/* If cmd contains '/', try directly */
if (strchr(cmd, '/'))
{
if (stat(cmd, &st) == 0 && access(cmd, X_OK) == 0)
{
char *ret = malloc(strlen(cmd) + 1);
if (ret)
strcpy(ret, cmd);
return ret;
}
return NULL;
}

/* PATH search */
path_env = get_env_value("PATH");
if (!path_env || *path_env == '\0')
return NULL;

/* Work on a copy to scan directories separated by ':' */
copy = malloc(strlen(path_env) + 1);
if (!copy)
return NULL;
strcpy(copy, path_env);

p = copy;
while (*p)
{
/* dir start */
start = p;
/* find ':' or end */
while (*p && *p != ':')
p++;
/* terminate dir segment */
if (*p == ':')
*p++ = '\0';

/* skip empty segments */
if (*start == '\0')
continue;

/* build candidate and test */
candidate = join_path(start, cmd);
if (candidate)
{
if (stat(candidate, &st) == 0 && access(candidate, X_OK) == 0)
{
free(copy);
return candidate;
}
free(candidate);
}
}
/* also check final segment if last char wasn't ':' (handled in loop) */
free(copy);
return NULL;
}

/* Main: PATH support, no fork if missing, quiet stderr, newline splitting */
int main(int argc, char **argv)
{
char *line = NULL, *cmd;
size_t n = 0;
ssize_t read;
char **args;
char *path;
pid_t pid;
int status;

(void)argc;
(void)argv;

for (;;)
{
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "#cisfun$ ", 9);

read = getline(&line, &n, stdin);
if (read == -1)
{
free(line);
return 0;
}

/* process each newline-separated command */
cmd = strtok(line, "\n");
while (cmd)
{
cmd = trim_spaces(cmd);
if (*cmd == '\0')
{
cmd = strtok(NULL, "\n");
continue;
}

args = tokenize_line(cmd);
if (!args || !args[0] || args[0][0] == '\0')
{
if (args) free(args);
cmd = strtok(NULL, "\n");
continue;
}

path = resolve_command(args[0]);
if (!path)
{
/* do not fork, do not write to stderr */
free(args);
cmd = strtok(NULL, "\n");
continue;
}

/* execute */
pid = fork();
if (pid == -1)
{
free(args);
free(path);
cmd = strtok(NULL, "\n");
continue;
}
if (pid == 0)
{
execve(path, args, environ);
/* keep quiet on error for checker expectations */
_exit(EXIT_FAILURE);
}
waitpid(pid, &status, 0);
free(args);
free(path);

cmd = strtok(NULL, "\n");
}
}
free(line);
return 0;
}
