/*********************************************************************
   PROGRAM:    CSCI 330 Assignment 3
   PROGRAMMER: Cory Stojan
   LOGON ID:   z1804761
   DUE DATE:   13FEB18
 
   FUNCTION:   
*********************************************************************/

// Include Statements
#ifndef FUNCTIONS_H
#define FUNCTIONS_
#include "functions.h"
#include <string>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*
read(2)
write(2)
getopt(3)
*/
// inital phase to encrypt the data. the phase shift works but the hex and binary output dont
void encrypt(char *stringBuffer, int binaryShift, int phaseShift, int binary, int hex, int bytesRead)
{
	// apply the phase shift to the cesar cypher
	if (phaseShift > 0)
	{
		stringBuffer = cesarShift(stringBuffer, phaseShift);
		
	}
	
	// binary and hex are outputs (create functios for each)
	else if (binary == 1 || hex == 1)
	{
		output(stringBuffer, binaryShift, hex, binary, bytesRead);
		cout << endl;
	}
	else 
	printf(stringBuffer);
}

// text to binary conversion/ output
void output(char *stringBuffer, int binaryShift, int hex, int binary, int bytesRead)
{
	int i = 0;
	while (stringBuffer[i] < bytesRead)
	{
		
		int value = (int)stringBuffer[i];
		to_bin(value);
		i++;
	}
	
}
void to_bin(int value)
{
    char base_range[] = "01";

    if (value >= 2) {
        to_bin(value / 2);
    }

    printf("%c", base_range[value % 2]);
}
// the cesar shift. Takes the buffer string filled when opening a file and shifts it alphabetically
// up by the number specified by the user.
char* cesarShift(char *stringBuffer, int phaseShift)
{
	for (int i = 0; stringBuffer[i] != '\0'; i++)
		{
			int asci;
			asci = (int)stringBuffer[i];
			// alphabetical values
			if (((asci >= 65) && (asci <= 90)) || ((asci >=97) && (asci <= 122)))
			{
				// reduce phase shift to lowest multipul of 26
				while (phaseShift > 26)
				{
					phaseShift = phaseShift - 26;
				}

				// apply phase shift to asci values
				asci = asci + phaseShift;
			

				// upper case shift asci values (range 65-90)
				if ((int)stringBuffer[i] <= 90 && (int)stringBuffer[i] >= 65)
				{
					if (asci > 90)
						asci = asci - 26;

					stringBuffer[i] = (char)asci;
				}

				// lower case shift asci value (range 97-122)
				if ((int)stringBuffer[i] <= 122 && (int)stringBuffer[i] >= 97)
				{
					if (asci > 122)
						asci = asci - 26;
					
					stringBuffer[i] = (char)asci;
				}
			}
		}
	return stringBuffer;
}


#endif /*FUNCTIONS_H*/
