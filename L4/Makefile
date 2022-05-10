CC=g++ -ggdb3

main: main.o bitrw.o bitrw.hh stats.o stats.hh tga.o tga.hh jpeg-ls.o jpeg-ls.hh


main.o:
	g++ -ggdb3 -c -o main.o main.cc

bitrw.o:
	g++ -ggdb3 -c -o bitrw.o bitrw.cc

stats.o:
	g++ -ggdb3 -c -o stats.o stats.cc

tga.o:
	g++ -ggdb3 -c -o tga.o tga.cc

jpeg-ls.o:
	g++ -ggdb3 -c -o jpeg-ls.o jpeg-ls.cc

clean:
	rm *.o main
