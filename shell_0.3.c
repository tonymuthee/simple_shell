#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

void shell_prompt(void)
{
printf("simple_shell> ");
}

void execute_command(char *command, char *args[])
{
execvp(command, args);
perror("simple_shell");
exit(EXIT_FAILURE);
}

int main(void)
{
char input[MAX_INPUT_LENGTH];
char *args[MAX_ARGS];
while (1)
{
shell_prompt();
if (fgets(input, sizeof(input), stdin) == NULL)
{
if (feof(stdin))
{
printf("\n");
break;
}
}
input[strcspn(input, "\n")] = '\0';
int arg_count = 0;
char *token = strtok(input, " ");
while (token != NULL && arg_count < MAX_ARGS - 1)
{
args[arg_count++] = token;
token = strtok(NULL, " ");
}
args[arg_count] = NULL;
if (arg_count > 0)
{
char *command_path = args[0];
if (access(command_path, F_OK) == -1)
{
char *path = getenv("PATH");
if (path != NULL)
{
char *path_token = strtok(path, ":");
while (path_token != NULL)
{
char full_path[MAX_INPUT_LENGTH];
snprintf(full_path, sizeof(full_path), "%s/%s", path_token, args[0]);
if (access(full_path, F_OK) == 0)
{
command_path = full_path;
break;
}
path_token = strtok(NULL, ":");
}
}
}
pid_t pid = fork();
if (pid == -1)
{
perror("simple_shell");
exit(EXIT_FAILURE);
}
else if (pid == 0)
{
execute_command(command_path, args);
}
else
{
int status;
waitpid(pid, &status, 0);
}
}
}
return (0);
}

