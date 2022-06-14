CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

all: clean main encode decode compare


tests: main.py
	for quant in {1..7} \
	do \
		for img in 0 1 2 3 \
		do \
			INPUT := $(addsuffix $(addsuffix .tga,$$img),testy/example) \
			OUTPUT := output/kikd.kkd \
			RESULT := $(addsuffix $$quant,$(addsuffix $$img,results/)) \
			ERROR := $(addsuffix $(addsuffix .txt,$$quant),$(addsuffix $$img,errors/)) \
			python3 main.py -e $(INPUT) $(OUTPUT) $$quant \
			python3 main.py -d $(OUTPUT) $(RESULT) \
			python3 main.py -c $(INPUT) $(addsuffix _new.tga,$(RESULT)) >$(ERROR) \
		done \
	done


main: main.o main.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
test: test.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
encode: encode.o main.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
decode: decode.o main.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh
compare: compare.o main.hh bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh pixel.o pixel.hh differential.o differential.hh

run0: main
	./main testy/example0.tga output/kikd.kkd 3

run1: main
	./main testy/example1.tga output/kikd.kkd 4

run2: main
	./main testy/example2.tga output/kikd.kkd 5

run3: main
	./main testy/example3.tga output/kikd.kkd 6

val: main
	valgrind --leak-check=full --track-origins=yes ./main testy/example1.tga output/kikd.kkd 4

test.run: test
	./test
	python3 create_png.py output/example0.txt output/example0.tga

test.val: test
	valgrind --leak-check=full --track-origins=yes ./test

clean:
	rm -rf *.o main test output/*
