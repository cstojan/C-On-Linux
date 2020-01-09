//*****************************************************************
// FILE:      CesarShift.h
// AUTHOR:    Cory Stojan
// LOGON ID:  Z1804761
// DUE DATE:  24FEB18
//
// PURPOSE:   Contains the hedder file for the cesar class.
//*****************************************************************

// Header files ( including headder guards)
#ifndef CESAR_H
#define CESAR_H

using namespace std;
	
void encrypt(char*, int, int, int, int, int);
char binaryShift(char, int );
void output(char *, int, int, int, int);
char* cesarShift(char *, int);
void to_bin(int);
// Headder guard
#endif /*CESAR_H*/
