/*********************************************************************
   PROGRAM:    CSCI 330 Assignment 3
   PROGRAMMER: Cory Stojan
   LOGON ID:   z1804761
   DUE DATE:   13FEB18
 
   FUNCTION:   
*********************************************************************/

// Include Statements
#include "functions.h"
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <errno.h>
#include <getopt.h>



using namespace std;

// main function

int main(int argc, char *argv[])
{
	
	// Local Variables
	ssize_t howmany;
	
		// Default Values
		int buffer = 1024;
		int phaseShift = 0;
		int binaryShift = 0;
		int bytesRead = 1024;
		int c = 0, hex = 0, binary = 0, fd = 0;
	//	struct stat strBuf;
		char fileName[10] = " ";
  		

	strcpy(fileName, argv[1]);
    cout << fileName << endl << endl;
  while ((c = getopt(argc, argv, "s:n:c:r:xb")) != -1) 
  {
    switch (c)
	{
    case 's': // buffer
      buffer = atoi(optarg);
      break;
      
    case 'n': // bytes read
      bytesRead = atoi(optarg);
      break;
      
      case 'c': // shift (regular)
      phaseShift = atoi(optarg);
      break;
      
      case 'r': // binary shift
      binaryShift = atoi(optarg);
      break;
      
      case 'x': // hex output
    	hex = 1;
      break;
      
      case 'b': // binary output
    	binary= 1;
      break;
      
      case '?': // unknown variable
      	// add unknown cases
      break;
      	
      //default:
        //add exit
    	}
  }
  	if (binary == 1 && hex == 1)
			{
				cout << "invalid output arguments" << endl;
				return 1;
			}
	if (phaseShift > 0 && binaryShift > 0)
	{
		cout << "invalid output arguments" << endl;
				return 1;
	}
		// intialize the buffer
		char stringBuffer[buffer] = " ";

		// open the file
		fd = open(fileName, O_RDWR);
		
		// cheeck for errors
		if (fd == -1)
		{
			perror("Error while Opening File ");
			close(fd);
			if (close(fd) == -1)
			{
				perror("Error while closing file: open");
				exit(1);
			}
		}

		else
		{
			// read the file
			howmany = read(fd, stringBuffer, bytesRead);
			if(howmany == -1)
			{
				perror("reading file"); exit(1);
			}
			cout << "bytes read " << howmany << endl;
			cout << "buffer Size " << buffer << endl << endl;
		}
		
		encrypt(stringBuffer, binaryShift, phaseShift, binary, hex, bytesRead);
		

		//close file
		close(fd);

		//go to the next file (accout for arguments from previous file)
		//i = i + arguments;
	
	return 0;
}

