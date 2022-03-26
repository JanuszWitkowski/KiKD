all: clean main

main: main.o entropy.o cabac.o
	# 

entropy.o: entropy.cc entropy.hh
	g++ entropy.hh entropy.cc -o entropy.o

cabac.o: cabac.cc cabac.hh
	# 

clean: glowny
	rm glowny
