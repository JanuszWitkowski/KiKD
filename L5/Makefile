CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

main: main.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh quantizer.o quantizer.hh
test: test.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh quantizer.o quantizer.hh

val: main
	valgrind --leak-check=full --track-origins=yes ./main testy/example2.tga output/example.tga 0

run0: main
	./main testy/example0.tga output/example.tga 4
	python3 create_png.py

run1: main
	./main testy/example1.tga output/example.tga 4
	python3 create_png.py

run2: main
	./main testy/example2.tga output/example.tga 5
	python3 create_png.py

run3: main
	./main testy/example3.tga output/example.tga 7
	python3 create_png.py

test.run: test
	./test
	python3 create_png.py

test.val: test
	valgrind --leak-check=full --track-origins=yes ./test

clean:
	rm *.o main test
