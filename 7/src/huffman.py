

#!/bin/bash

from sys import argv, exit
from datetime import datetime


class Node():

    def __init__(self, fp, sp):
        self.fp = fp
        self.sp = sp


def huffman(node, current_code=''):
    if type(node) is str:
        return {node: current_code}
    current, left, right = dict(), huffman(node.fp, current_code + '0'), huffman(node.sp, current_code + '1')
    for key in left:
        current[key] = left[key]
    for key in right:
        current[key] = right[key]
    return current

def btoi(binary: str) -> int:
    result = 0
    for i in range(len(binary)):
        result += (2 ** (len(binary) - i - 1)) * int(binary[i])
    return result

def encode():

    startTime = datetime.now().second * 6 + datetime.now().microsecond

    if len(argv) < 4:
        print("Not enough arguments")
        exit(1)
    
    data = open(argv[2]).read()

    dictionary = dict()
    for symbol in data:
        if symbol in dictionary.keys():
            dictionary[symbol] += 1
        else:
            dictionary[symbol] = 1

    dictionary = sorted(dictionary.items(), key=lambda x: x[1], reverse=True)

    saved_copy = dictionary.copy()
    while len(dictionary) - 1:
        last, previous, dictionary = dictionary[-1], dictionary[-2], dictionary[:-2]
        dictionary += [(Node(last[0], previous[0]), last[1] + previous[1])]
        dictionary = sorted(dictionary, key=lambda x: x[1], reverse=True)

    codes = huffman(dictionary[0][0])

    print("Codes are:")
    for char in list(map(lambda x: x[0], saved_copy)):
        print(f"{char} {codes[char]}")
    
    outputFile = open(argv[3], "wb")

    outputFile.write(str(sum([len(codes[saved_copy[i][0]]) * saved_copy[i][1] for i in range(len(saved_copy))])).encode())
    outputFile.write(chr(128).encode())
    for symbol in list(map(lambda x: x[0], saved_copy)):
        outputFile.write(f"{symbol}{len(codes[symbol])}{chr(btoi(codes[symbol]))}".encode())
    outputFile.write(chr(128).encode())

    buffer = ""

    for symbol in open(argv[2], "r").read():
        buffer += codes[symbol]
        if len(buffer) >= 7:
            outputFile.write(chr(btoi(buffer[:7])).encode())
            buffer = buffer[7:]
    if buffer != "":
        buffer += "0" * (8 - len(buffer))
    outputFile.write(chr(btoi(buffer[:7])).encode())

    outputFile.close()

    endTime = datetime.now().second * 6 + datetime.now().microsecond

    print(f"Finished in {endTime - startTime}μss. Coef is {len(open(argv[2]).read()) / len(open(argv[3]).read())}")

def ctob(length: int, symbol: str) -> str:
    result = bin(ord(symbol))[2:]
    while len(result) < length:
        result = "0" + result
    return result

def itob(length: int, number: int) -> str:
    result = bin(number)[2:]
    while len(result) < length:
        result = "0" + result
    return result

def decode():
    
    if len(argv) < 3:
        print("Not enough arguments")
        exit(1)

    data = open(argv[2], "rb").read().split(chr(128).encode())
    number, codes, data = data[0], data[1], data[2:]
    number = int(number.decode())
    data = chr(128).encode().join(data)

    codes = {chr(codes[i]): itob(int(chr(codes[i + 1])), codes[i + 2]) for i in range(0, len(codes), 3)}

    data = "".join([itob(7, data[i]) for i in range(len(data))])

    while data != "" and number != 0:
        for symbol in codes.keys():
            if data.startswith(codes[symbol]):
                print(symbol, end="")
                data = data[len(codes[symbol]):]
                number -= len(codes[symbol])
                break
    print()

if __name__ == "__main__":

    if (len(argv) < 2):
        print("Not enough arguments")
        exit(1)

    if (argv[1] == "-e"):
        encode()
    elif (argv[1] == "-d"):
        decode()
    else:
        print("Unknown flag")
        exit(1)
