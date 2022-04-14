all: clean main

main: main.o bitrw.o stats.o universal.o
	g++ main.o bitrw.o stats.o universal.o -o main

main.o:
	g++ -c main.cc

bitrw.o:
	g++ -c bitrw.cc

stats.o: stats.cc stats.hh
	g++ -c stats.cc

universal.o: universal.cc universal.hh
	g++ -c universal.cc

# test: main testy/test3.bin
# 	clear
# 	./main testy/test3.bin

clean:
	rm -rf main *.o
