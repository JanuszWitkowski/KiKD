CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

all: clean main encode decode compare

main: main.o main.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
test: test.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
encode: encode.o main.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
decode: decode.o main.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
compare: compare.o main.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh

run: main
	./main testy/example0.tga output/kikd.kkd 3

val: main
	valgrind --leak-check=full --track-origins=yes ./main testy/example0.tga output/kikd.kkd 4

test.run: test
	./test
	python3 create_png.py output/example0.txt output/example0.tga

test.val: test
	valgrind --leak-check=full --track-origins=yes ./test

clean:
	rm -rf *.o main test output/*
