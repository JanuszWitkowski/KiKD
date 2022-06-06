CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

main: main.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
test: test.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh

test.run: test
	./test

test.val: test
	valgrind --leak-check=full --track-origins=yes ./test

clean:
	rm -rf *.o main test output/*
