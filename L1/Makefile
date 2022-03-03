all: clean compile

compile: entropy.cc
	g++ entropy.cc -o entropy

test0: entropy
	./entropy testy/pan-tadeusz-czyli-ostatni-zajazd-na-litwie.txt

test1: entropy
	./entropy testy/test1.bin

test2: entropy
	./entropy testy/test2.bin

test3: entropy
	./entropy testy/test3.bin

clean: entropy
	rm entropy

debug: clean compile test0
