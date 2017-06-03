OBJS = ALU.cpp instruction.cpp programme.cpp processeur.cpp moveInstructionDecoder.cpp jumpInstructionDecoder.cpp main.cpp 
CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -Weffc++ -Werror

executable: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o executable
clean:
	\rm *.o *~ executable
