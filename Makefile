FLAGS = -Werror -std=c++11 -g
main: main.o game.o tetromino.o console.o util.o
	g++ $(FLAGS) -o main main.o game.o tetromino.o util.o console.o -lncurses
main.o: main.cpp game.h tetromino.h
	g++ $(FLAGS) -c -o main.o main.cpp
game.o: game.cpp game.h tetromino.h
	g++ $(FLAGS) -c -o game.o game.cpp
tetromino.o: tetromino.cpp tetromino.h
	g++ $(FLAGS) -c -o tetromino.o tetromino.cpp
console.o: console/console.cpp console/console.h
	g++ $(FLAGS) -c -o console.o console/console.cpp -lncurses
util.o: util.cpp util.h tetromino.h
	g++ $(FLAGS) -c -o util.o util.cpp
