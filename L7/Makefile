CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

szum: szum.o bitrw.o bitrw.hh

szum.run: szum
	./szum 0.5 szum.cc output/szum.cc

szum.cmp: szum.run
	xxd szum.cc output/szum_in.hex
	xxd output/szum.cc output/szum_out.hex
	diff output/szum_in.hex output/szum_out.hex

clean:
	rm -rf *.o output/*.hex szum