.PHONY: all compile run d e python

python:
	python3 src/huffman.py -d data/output.txt

all: compile run

e:
	python3 src/huffman.py -e data/input.txt data/output.txt

d:
	python3 src/huffman.py -d data/output.txt

compile:
	gcc src/huffman.c -o bin/huffman

run:
	bin/huffman data/input.txt data/output.txt
