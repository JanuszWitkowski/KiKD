all: clean main

main: main.o entropy.o universal.o
	g++ main.o entropy.o universal.o -o main

main.o:
	g++ -c main.cc

entropy.o: entropy.cc entropy.hh
	g++ -c entropy.cc

universal.o: universal.cc universal.hh
	g++ -c universal.cc

# test: main testy/test3.bin
# 	clear
# 	./main testy/test3.bin

clean:
	rm -rf main *.o
