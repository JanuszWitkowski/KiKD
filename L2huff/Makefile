all: clean main

main: main.o entropy.o huff.o
	g++ main.o entropy.o huff.o -o main

main.o:
	g++ -c main.cc

entropy.o: entropy.cc entropy.hh
	g++ -c entropy.cc

huff.o: huff.cc huff.hh
	g++ -c huff.cc

clean:
	rm -rf main *.o
