/*********************************************************************
   PROGRAM:    CSCI 230 Assignment 2
   PROGRAMMER: Cory Stojan
   LOGON ID:   z1804761
   DUE DATE:   2/09/2018
 
   FUNCTION:   This program acts like a cat command.
*********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

// Allow for multiple arguments
int main(int argc, char *argv[])
{
	// Allow for streaming in file data and sotring it.
	string input;
	fstream nameFile;
	
	// Open and Read and Display the contents of the file.
	for (int i = 1; i <= argc; i++) {
	
		// open a file
		nameFile.open(argv[i], ios::in);
	
		// if its ipens correctly
		if(nameFile) {
			
			// reads the first line of the file.
			getline(nameFile, input);
		
			while(nameFile) {
				cout << input << endl; // outputs the current line of the file
				getline(nameFile, input);  // moves to the next line.
			}
		
		}
		
		// Print error if the file incorrectly opens
		else {
			perror("Error while opening file"); 
			exit(1);
		}
	nameFile.close();
	}
	return 0;
}


