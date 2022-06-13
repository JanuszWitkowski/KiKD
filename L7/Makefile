CC=g++
CXXFLAGS = -ggdb3
LDFLAGS = -ggdb3

koder: koder.o bitrw.o bitrw.hh
szum: szum.o bitrw.o bitrw.hh
dekoder: dekoder.o bitrw.o bitrw.hh
sprawdz: sprawdz.o bitrw.o bitrw.hh

run: koder szum dekoder sprawdz
	./koder testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/kikd.kkd
	./szum 0.0005 output/kikd.kkd output/kikd_szum.kkd
	./dekoder output/kikd_szum.kkd output/pan-tadzio.txt
	./sprawdz testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/pan-tadzio.txt

xxd: testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/pan-tadzio.txt
	xxd testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/p1.hex
	xxd output/pan-tadzio.txt output/p2.hex
	diff output/p1.hex output/p2.hex

koder.run: koder
	./koder koder.cc output/koder.kkd

szum.run: szum
	./szum 0.5 szum.cc output/szum.cc

szum.test: szum.run
	xxd szum.cc output/szum_in.hex
	xxd output/szum.cc output/szum_out.hex
	diff output/szum_in.hex output/szum_out.hex

dekoder.run: koder dekoder sprawdz
	./koder testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/kikd.kkd
	./dekoder output/kikd.kkd output/pan-tadzio.txt
	./sprawdz testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/pan-tadzio.txt

sprawdz.run: sprawdz
	./sprawdz sprawdz.cc szum.cc

sprawdz.test: sprawdz szum
	./szum 0.001 sprawdz.cc output/sprawdz.cc
	./sprawdz sprawdz.cc output/sprawdz.cc

clean:
	rm -rf *.o output/* koder szum dekoder sprawdz
