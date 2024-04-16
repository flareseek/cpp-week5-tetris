FLAGS = -Werror -std=c++11 -g
LINKER_FLAGS = $(FLAGS)
CC = g++

ifneq ($(OS),Windows_NT)
	LINKER_FLAGS += -lncurses
endif

all: main

main: main.o game.o tetromino.o console.o util.o
	$(CC) $(LINKER_FLAGS) -o main main.o game.o tetromino.o console.o util.o

console.o: console/console.cpp console/console.h
	$(CC) $(FLAGS) -c -o console.o console/console.cpp

main.o: main.cpp game.h tetromino.h
	$(CC) $(FLAGS) -c -o main.o main.cpp

game.o: game.cpp game.h tetromino.h util.h
	$(CC) $(FLAGS) -c -o game.o game.cpp

tetromino.o: tetromino.cpp tetromino.h util.h console/console.h
	$(CC) $(FLAGS) -c -o tetromino.o tetromino.cpp

util.o: util.cpp util.h tetromino.h
	g++ $(FLAGS) -c -o util.o util.cpp

clean:
	rm -f *.o main.exe main
