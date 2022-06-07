CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

main: main.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
test: test.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh

test.run: test
	./test
	python3 create_png.py output/example0.txt output/example0.png

test.val: test
	valgrind --leak-check=full --track-origins=yes ./test

clean:
	rm -rf *.o main test output/*
