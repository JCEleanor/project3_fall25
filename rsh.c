#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp", "touch", "mkdir", "ls", "pwd", "cat", "grep", "chmod", "diff", "cd", "exit", "help"};

int isAllowed(const char *cmd)
{
	for (int i = 0; i < N; i++)
	{
		if (strcmp(cmd, allowed[i]) == 0)
		{
			return 1;
		}
	}
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise

	return 0;
}

int main()
{

	// TODO
	// Add variables as needed

	char line[256];

	while (1)
	{

		fprintf(stderr, "rsh>");

		if (fgets(line, 256, stdin) == NULL)
			break;

		if (strcmp(line, "\n") == 0)
			continue;

		line[strlen(line) - 1] = '\0';

		char *argv[21];
		char *token;
		// argument count
		int argc = 0;
		token = strtok(line, " ");

		// limit to 20 arguments
		while (token != NULL && argc < 20)
		{
			argv[argc] = token;
			argc++;
			token = strtok(NULL, " ");
		}

		// add NULL at the end of argv
		argv[argc] = NULL;

		// case 1: no arguments. i.e., user presses enter to entered only spaces
		if (argc == 0)
		{
			continue;
		}

		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		if (strcmp(argv[0], "exit") == 0)
		{
			break;
		}
		else if (strcmp(argv[0], "help") == 0)
		{
			printf("The allowed commands are:\n");
			for (int i = 0; i < N; i++)
			{
				printf(" - %s\n", allowed[i]);
			}
		}
		else if (strcmp(argv[0], "cd") == 0)
		{
			if (argc > 2)
			{
				// NOTE: you must print this error message in **standard output** instead of standard error for autograding purposes
				// fprintf(stderr, "-rsh: cd: too many arguments\n");
				printf("-rsh: cd: too many arguments\n");
			}
			else if (argc == 1)
			{
				// cd to home dir
				chdir(getenv("HOME"));
			}
			else if (argc == 2)
			{
				if (chdir(argv[1]) != 0)

				{
					// if the dir does not exist or other errors
					perror("-rsh: cd");
				}
			}
		}
		else
		{
			// process spwanning for other commands
			if (isAllowed(argv[0]))
			{
				pid_t pid;
				int status;
				posix_spawnattr_t attr;

				// Initialize spawn attributes
				posix_spawnattr_init(&attr);

				// Spawn a new process
				if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0)
				{
					perror("spawn failed");
					exit(EXIT_FAILURE);
				}
				else
				{
					if (waitpid(pid, &status, 0) == -1)
					{
						perror("waitpid failed");
						exit(EXIT_FAILURE);
					}
				}
			}
			// commands is not built-in (the shell executes the command within its current process.)
			else
			{
				printf("NOT ALLOWED!\n");
			}
		}

		// Use the example provided in myspawn.c
	}
	return 0;
}
