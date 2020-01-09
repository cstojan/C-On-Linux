#
# PROGRAM:    assign3
# PROGRAMMER: Cory Stojan
# LOGON ID:   z1804761
# DATE DUE:   23FEB18
#

# Compiler variables
CCFLAGS = -Wall -std=c++11

# Rule to link object code files to create executable file
assign3: assign3.o functions.o
	g++ $(CCFLAGS) -o assign3 assign3.o functions.o

# Rules to compile source code files to object code
assign3.o: assign3.cpp functions.h
	g++ $(CCFLAGS) -c assign3.cpp

functions.o: functions.cpp functions.h
	g++ $(CCFLAGS) -c functions.cpp

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o assign3
