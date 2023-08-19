#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

void shell_prompt(void)
{
printf("#cisfun$> ");
}

void execute_command(char *command)
{
char *args[] = {command, NULL};
execve(command, args, NULL);
perror("#cisfun$");
exit(EXIT_FAILURE);
}

int main(void)
{
char input[MAX_INPUT_LENGTH];
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

pid_t pid = fork();
if (pid == -1)
{
perror("#cisfun$");
exit(EXIT_FAILURE);
}
else if (pid == 0)
{
	execute_command(input);
} else
{
	int status;
waitpid(pid, &status, 0);
}
}
return (0);
}

