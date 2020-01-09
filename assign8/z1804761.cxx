/********************************************
   
   Programmer: Cory Stojan
   Class: CSCI 330
   Program Number: Assignment 7 
   Due Date: 04 May 18
   Purpose: Server program that continues listening until terminated.
        Process client request as path beyond root and shows directories, 
        index file if present or opens specified file in path.

********************************************/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <sys/uio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

using std::cout;
using std::endl;

int main (int argc, char ** argv)
{
	//Check for correct arguments and display usage instructions
	if (argc < 2) {
		cout << "Incorrect Arguments\n"
			 << "Correct usage is: 'port number' 'root directory'" << endl;
			 exit(EXIT_FAILURE);
	}

	int 	
		sock,		//Socket file descriptor 
		received;	//# data sent by client

	unsigned int	
		serverlen,	//Length of server address
		clientlen;	//Length of client address

	struct sockaddr_in 	
				echoserver, 	//Server address
				echoclient;	//client address
	
	DIR *dirp;			//Pointer to directory 
	struct dirent *dirEntry;	//Pointer to directory entry

	//Opens directory specified in arg 2 
	if ((dirp = opendir(argv[2])) == 0) { 
		perror(argv[2]); 
		exit(EXIT_FAILURE);
	}
	closedir(dirp);

	//Create the TCP socket and Error Checking
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failed to create socket");
		exit (EXIT_FAILURE);
	}

	//Construct the server sockaddr_in structure
	memset(&echoserver, 0, sizeof(echoserver));	//Clear struct
	echoserver.sin_family = AF_INET;		//Internet IP
	echoserver.sin_addr.s_addr = INADDR_ANY;	//IP Adress
	echoserver.sin_port = htons(atoi(argv[1]));	//Server Port

	//Bind the socket and Error Checking
	serverlen = sizeof(echoserver);
	if (bind(sock, (struct sockaddr *) &echoserver, serverlen) < 0) {
		perror ("Failed to bind server socket");
		exit(EXIT_FAILURE);
	}

	//Listen for incoming connection request and Error Checking
	if (listen(sock, 64) < 0) {
		perror("Listen Failed");
		exit(EXIT_FAILURE); 
	}
	
	//Accepts client request to connect and assigns client sock fd	
	while(int newSock = accept(sock,(struct sockaddr*)&echoclient, &clientlen)) 
	{ 
		if (fork()) { 		//Parent process
			close(newSock);	//Closes connection with client to keep listening
		}
		else 
		{ 	//child process / comms with connected client

			//Output on server side with client information
			cout << "Client connected: " << inet_ntoa(echoclient.sin_addr) << endl;
			
			//Redirect stdout to client
			if (dup2(newSock,1) < 0) { 
				perror("socket dup2"); 
				exit(EXIT_FAILURE);
			}
			
			//Redirect stderr to client
			if (dup2(newSock,2) < 0) { 
				perror("socket dup2"); 
				exit(EXIT_FAILURE);
			}				
			
			//Keeps connection open
			while(true)
			{
				int check = 1;
				char buffer[256]; 
				char path[50];
				char fullPath[50];
				int count = 0;		
				
				//Read a message from the client
				if ((received = read(newSock, buffer, 256)) < 0) {
					perror ("Failed to receive message");
					exit (EXIT_FAILURE);
				}
				buffer[received] = '\0';	//Null terminate client message
				
				//Separates path from client input/message
				while(buffer[count+5] != '\0')
					{
						path[count] = buffer[count+5];
						count++;
					}
					
				//Check for substring in path
				for (int i = 0; i < count; i++)
				{
					if (path[i] == '.')
					{
						if (path[i+1] == '.'){
							cout << "Can not contain substring" << endl;
							check = 0;
							break;
						}
					}
				}	
				
				//Checks for correct "trigger" input command
				if (!strncmp(buffer,"GET /",5) && check == 1) 
				{	
				//Continues only if valid directory or filename charecter
					if (isalpha(buffer[5]) || isdigit(buffer[5]) || buffer[5] == '_' || buffer[5] == '-') 
					{

						//Combine client path with web server root path
						strcpy(fullPath, argv[2]);
						path[count-1] = '\0';
						strcat(fullPath, path);
						
						//Remove trailing newline
						int len = strlen(fullPath);
							fullPath[len-1] = '\0';				

						//Open directory specified by client	
						if ((dirp = opendir(fullPath)) == 0) { 
							if (errno == 20)
								execlp("less","less", fullPath,(char*)(NULL));
							else
							{
								perror(fullPath); 
								exit(EXIT_FAILURE);
							}
						}
						
						//Search DIR for index file
						while((dirEntry = readdir(dirp)) != NULL)
						{
							//Transmits each entry to the client
							if (!strcmp(dirEntry->d_name, "index.html"))	
								execlp("less","less", "index.html",(char*)(NULL));
						}
						closedir(dirp);
						
						//Open directory specified by client	
						if ((dirp = opendir(fullPath)) == 0) { 
							perror(fullPath); 
							exit(EXIT_FAILURE);}
						
						while((dirEntry = readdir(dirp)) != NULL)
						{
						
						//Transmits each entry to the client							
							if (write(newSock,dirEntry->d_name,strlen(dirEntry->d_name)) != (unsigned int)strlen(dirEntry->d_name))
								{perror("Unable to transmit, please try again"); exit(EXIT_FAILURE);}
							cout << endl;
						}						
						closedir(dirp);	
					} 
	
					//End of client path dec tree
					//Searches through root of webserver specified in arg2					
					else if (check == 1)
					{
						cout << "IN SEARCH OF ROOT" << endl;
						if ((dirp = opendir(argv[2])) == 0) { 
							perror(argv[2]); 
							exit(EXIT_FAILURE);}
					
						while((dirEntry = readdir(dirp)) != NULL) 
						{
							//Check if file is index and send contents to client
							if (!strcmp(dirEntry->d_name, "index.html"))
							{
								execlp("less","less","index.html",(char*)(NULL)); 
							}
						}
						closedir(dirp);
						//Opens root directory specified in arg2
						if ((dirp = opendir(argv[2])) == 0) { 
							perror(argv[2]); 
							exit(EXIT_FAILURE);}
						
						while((dirEntry = readdir(dirp)) != NULL)
						{
							//Transmits each entry to the client
							if (strncmp(dirEntry->d_name, ".",1))
							{
								if (write(newSock,dirEntry->d_name,strlen(dirEntry->d_name)) != (unsigned int)strlen(dirEntry->d_name))
									{perror("Unable to transmit, please try again");exit(EXIT_FAILURE);}
								cout << endl;
							}
						}
					}
				
				//End of print root contents/index	
				}

				//End of GET dec tree
				else if (check != 0) 
				{
					if (strncmp(buffer,"GET /",5)) 
						cout << "Incorrect Usage: request should follow 'GET /' format" << endl;
				}

				cout << "\nEnter next request (GET Path): " << endl;
			}
		}

	close(newSock);
	
	}

return 0;
}
