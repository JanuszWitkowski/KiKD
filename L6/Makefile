CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

main: main.o consts.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh
test: test.o consts.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh

test.val: test
	valgrind --leak-check=full --track-origins=yes ./test

clean:
	rm -rf *.o main test
