all: clean main

main: main.o entropy.o dhc.o
	@ g++ main.o entropy.o dhc.o -o main

main.o:
	g++ -c main.cc

entropy.o: entropy.cc entropy.hh
	g++ -c entropy.cc

dhc.o: dhc.cc dhc.hh
	g++ -c dhc.cc

clean:
	rm -rf main *.o
