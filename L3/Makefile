all: clean main

main: main.o entropy.o cabac.o
	@ g++ main.o entropy.o cabac.o -o main

main.o:
	g++ -c main.cc

entropy.o: entropy.cc entropy.hh
	g++ -c entropy.cc

cabac.o: cabac.cc cabac.hh
	g++ -c cabac.cc

test: main testy/test3.bin
	clear
	./main testy/test3.bin

clean:
	rm -rf main *.o
