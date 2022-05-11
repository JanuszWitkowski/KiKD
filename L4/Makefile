CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

main: main.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh jpeg-ls.o jpeg-ls.hh

val: main
	valgrind --leak-check=full --track-origins=yes ./main testy/example1.tga

clean:
	rm *.o main
