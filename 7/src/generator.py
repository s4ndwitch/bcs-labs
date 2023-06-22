#!/usr/bin/python3

from random import choice

alphabet = list("asdf")

with open("data/input.txt", "w") as input_file:
    for _ in range(100000):
        input_file.write(choice(alphabet))
