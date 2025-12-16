#include "shell.h"

/**
 * is_executable - Check if file is executable
 * @path: Path to file
 *
 * Return: 1 if executable, 0 otherwise
 */
static int is_executable(const char *path)
{
struct stat st;

return (stat(path, &st) == 0 && access(path, X_OK) == 0 && S_ISREG(st.st_mode));
}

/**
 * find_in_path - Resolve command using PATH
 * @cmd: Command name
 *
 * Return: Full path string or NULL
 */
char *find_in_path(const char *cmd)
{
char *path, *copy, *dir, *full;
size_t len;

if (!cmd)
return (NULL);

path = getenv("PATH");
if (!path)
return (NULL);

copy = strdup(path);
if (!copy)
return (NULL);

dir = strtok(copy, ":");
while (dir)
{
len = strlen(dir) + 1 + strlen(cmd) + 1;
full = malloc(len);
if (!full)
{
free(copy);
return (NULL);
}
sprintf(full, "%s/%s", dir, cmd);
if (is_executable(full))
{
free(copy);
return (full);
}
free(full);
dir = strtok(NULL, ":");
}
free(copy);
return (NULL);
}
