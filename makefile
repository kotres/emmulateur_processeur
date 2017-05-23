OBJS = ALU.cpp registres.cpp programme.cpp processeur.cpp main.cpp 
CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -Weffc++ -Werror

executable: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o executable
clean:
	\rm *.o *~ executable
