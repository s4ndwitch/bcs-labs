.PHONY: simple build all

all: compile input

input:
	build/main data/input.txt

compile: src/main.c build
	$(CC) -o build/main src/main.c

besy: compile build/main data/besy.pdf
	build/main data/besy.pdf

build:
	if test ! -d build; then mkdir build; fi

simple: src/simple.c build
	$(CC) -o build/simple src/simple.c
	build/simple data/input.txt
