#!/usr/bin/python3

from sys import argv

print(open(argv[2], "rb").read() == open(argv[1], "rb").read())
