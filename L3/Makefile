all: clean main

main: main.o bitrw.o stats.o universal.o lzw.o
	g++ -ggdb3 main.o bitrw.o stats.o universal.o lzw.o -o main
	rm *.o

main.o:
	g++ -ggdb3 -c main.cc

bitrw.o:
	g++ -ggdb3 -c bitrw.cc

stats.o: stats.cc stats.hh
	g++ -ggdb3 -c stats.cc

universal.o: universal.cc universal.hh
	g++ -ggdb3 -c universal.cc

lzw.o: lzw.cc lzw.hh
	g++ -ggdb3 -c lzw.cc

# test: main testy/test3.bin
# 	clear
# 	./main testy/test3.bin

clean:
	rm -rf main *.o
