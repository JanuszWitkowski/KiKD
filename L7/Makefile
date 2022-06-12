CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

koder: koder.o bitrw.o bitrw.hh
szum: szum.o bitrw.o bitrw.hh
sprawdz: sprawdz.o bitrw.o bitrw.hh

koder.run: koder
	./koder koder.cc output/koder.kkd

szum.run: szum
	./szum 0.5 szum.cc output/szum.cc

szum.test: szum.run
	xxd szum.cc output/szum_in.hex
	xxd output/szum.cc output/szum_out.hex
	diff output/szum_in.hex output/szum_out.hex

sprawdz.run: sprawdz
	./sprawdz sprawdz.cc szum.cc

sprawdz.test: sprawdz szum
	./szum 0.001 sprawdz.cc output/sprawdz.cc
	./sprawdz sprawdz.cc output/sprawdz.cc

clean:
	rm -rf *.o output/* szum
