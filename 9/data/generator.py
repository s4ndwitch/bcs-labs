#!/bin/python3

from random import choice
alphabet = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"

result = ""
while (len(result) < 100):
    result += choice(alphabet)

open("input.txt", "w").write(result)