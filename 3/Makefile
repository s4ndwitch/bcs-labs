.PHONY: all

all: build_folder src/main.c
	$(CC) -o build/main src/main.c -fno-stack-protector -g
	build/main

build_folder:
	if test ! -d build; then mkdir build; fi