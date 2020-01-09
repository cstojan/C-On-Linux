 /*********************************************************************
   PROGRAM:    CSCI 330 Assignment 4
   PROGRAMMER: Cory Stojan
   LOGON ID:   z1804761
   DUE DATE:   09MAR18
 
   FUNCTION: To get fimiliar with using fork, pipe, dup
*********************************************************************/

// include statements
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

// standard namespaces
using namespace std;

int main()
{
	// variables
	int Quit = 0;

//	loop
	while(Quit != -1)
	{
		// variables
		char instring1[127] = {" "};
		char instring2[127] = {" "};
		int mypipe[2];
		char *quit[2];
		int fd, pipefd;
		int fork1, fork2;

		// ask for commands for first command (stores values also)
		cout << "first sring" << endl;
		fgets(instring1, sizeof(instring1), stdin);  // Use fgets instead of gets.

		// asks for commands for secomd command (stores values also)
		cout << "second sring" << endl;
		fgets(instring2, sizeof(instring2), stdin);  // Use fgets instead of gets.

//		first fork
		fork1 = fork();

		// create pipe
		pipefd = pipe(mypipe);
		if (fd < 0) 
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		// Child process
 		if(fork1 == 0)
		{
			// second fork
			fork2 = fork();

			// ****** COMMAND LINE 1 *** ||
			if (fork2 > 0) // <-- child process
			{
				// close read end of pipe
				close(mypipe[0]);

				// close standard output
				close(1);

				// duplicate write end of pipe into standard output
				dup(mypipe[1]);

				// Create parent process variables
				char *Command1Array[6];
				char *command1 = NULL;
				int n = 0;

				// strtok
				command1 = strtok(instring1," ");

				// Extract all commands
				while (command1 != NULL)
				{
					Command1Array[n] = command1;
					command1 = strtok(NULL, " ");
					n++;
				}

				// look for the quit command
				for (int j = 0; j < 6; j++)
				{
					if ((strcmp(Command1Array[j], "quit") == 0)  || Quit == -1)
					{
						Quit = -1;
						cout << "Thanks for playing1!" << endl;
						return 0;
					}
				}

				// Execute commands
				fd = execvp(Command1Array[0], Command1Array);
				if (fd < 0)
				{
					perror("execvp Command1 error");
				}
			}

//			fork error message
			if (fork2 < 0)
			{
				perror("fork2 error");
				return 0;
			}
		}
		
		else // <-- parent process
		{
			// ***** COMMAND LINE 2 *** ||
			// close write end of pipe
			close(mypipe[1]);

			// duplicate read end of pipe into standard input
			dup(mypipe[0]);

			// Create child process variables
				char *Command2Array[6];
				char *command2 = NULL;
				int i = 0;

				// strtok
				command2 = strtok(instring2," ");

				// Extract all commands
				while (command2 != NULL)
				{
					Command2Array[i] = command2;
					command2 = strtok(NULL, " ");
					i++;
				}

//				search for quit command
				for (int j = 0; Command2Array[j] != "\0" && j < 6; j++)
				{
					
					if ((strcmp(Command2Array[j], "quit") == 0)  || Quit == -1)
					{
						Quit = -1;
						cout << "Thanks for playing2!" << endl;
						return 0;
					}
				}

				// wait for child
				wait(NULL);
				
				// Execute commands
				fd = execvp(Command2Array[0], Command2Array);
				if (fd < 0)
				{
					perror("execvp Command2 error");
				}
		}

		// flush the pipe?
		fflush(stdout);
		
		// close pipe
		fd = close(pipefd);
		if (fd < 0)
		{
			perror("error with closing pipe");
		}
	}

	return 0;
}

// segmation fault?

// needing a space after a command?

// no idea how to fix

// can i get feedback as to what i did wrong so i can learn from my mistakes?

