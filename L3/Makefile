all: clean main

main: main.o bitrw.o stats.o universal.o lzw.o
	@ g++ -ggdb3 main.o bitrw.o stats.o universal.o lzw.o -o main
	@ rm *.o

main.o:
	@ g++ -ggdb3 -c main.cc

bitrw.o:
	@ g++ -ggdb3 -c bitrw.cc

stats.o: stats.cc stats.hh
	@ g++ -ggdb3 -c stats.cc

universal.o: universal.cc universal.hh
	@ g++ -ggdb3 -c universal.cc

lzw.o: lzw.cc lzw.hh
	@ g++ -ggdb3 -c lzw.cc

test0: main
	./main -f testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	diff testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt

test1: main
	./main -f testy/test1.bin
	diff testy/test1.bin output/testy/test1.bin

test2: main
	./main -f testy/test2.bin
	diff testy/test2.bin output/testy/test2.bin

test3: main
	./main -f testy/test3.bin
	diff testy/test3.bin output/testy/test3.bin

stats: gamma delta omega fib

gamma: main
	./main -u gamma -f testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	./main -u gamma -f testy/test1.bin
	./main -u gamma -f testy/test2.bin
	./main -u gamma -f testy/test3.bin
	diff testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	diff testy/test1.bin output/testy/test1.bin
	diff testy/test2.bin output/testy/test2.bin
	diff testy/test3.bin output/testy/test3.bin

delta: main
	./main -u delta -f testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	./main -u delta -f testy/test1.bin
	./main -u delta -f testy/test2.bin
	./main -u delta -f testy/test3.bin
	diff testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	diff testy/test1.bin output/testy/test1.bin
	diff testy/test2.bin output/testy/test2.bin
	diff testy/test3.bin output/testy/test3.bin

omega: main
	./main -f testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	./main -f testy/test1.bin
	./main -f testy/test2.bin
	./main -f testy/test3.bin
	diff testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	diff testy/test1.bin output/testy/test1.bin
	diff testy/test2.bin output/testy/test2.bin
	diff testy/test3.bin output/testy/test3.bin

fib: main
	./main -u fib -f testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	./main -u fib -f testy/test1.bin
	./main -u fib -f testy/test2.bin
	./main -u fib -f testy/test3.bin
	diff testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt output/testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt
	diff testy/test1.bin output/testy/test1.bin
	diff testy/test2.bin output/testy/test2.bin
	diff testy/test3.bin output/testy/test3.bin

clean:
	@ rm -rf main *.o
