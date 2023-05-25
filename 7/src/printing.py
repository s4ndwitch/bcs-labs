#!/bin/python

from sys import argv

if __name__ == "__main__":
    if len(argv) < 3:
        exit(1)
    open(argv[1], "a").write(chr(int(argv[2])))
    exit(0)
