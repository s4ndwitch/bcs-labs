#!/bin/python3
from sys import argv

if argv[1] == '-e':
    result = 0
    for i in range(len(argv[2])):
        result += (2 ** (len(argv[2]) - i - 1)) * int(argv[2][i])
    print(result)
elif argv[1] == '-d':
    print(bin(argv[2])[2:])
