CPP=g++
FLAGS=-Wall -Wextra -std=c++17
LIBS=-lSDL2
INCLUDE=-I/usr/include/SDL2

Main: Main.o
	$(CPP) -o Main Main.o  $(LIBS)
Main.o: Main.cpp Utility.hpp
	$(CPP) -c $(FLAGS) Main.cpp Utility.hpp $(INCLUDE)
clean:
	rm *.o *.gch Main
