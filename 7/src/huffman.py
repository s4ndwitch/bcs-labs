#!/usr/bin/python3

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

    startTime = datetime.now().second * (10 ** 6) + datetime.now().microsecond

    if len(argv) < 4:
        print("Not enough arguments")
        exit(1)
    
    data = open(argv[2], "rb").read()

    dictionary = dict()
    for symbol in data:
        symbol = chr(symbol)
        if symbol in dictionary.keys():
            dictionary[symbol] += 1
        else:
            dictionary[symbol] = 1

    dictionary = sorted(dictionary.items(), key=lambda x: x[1], reverse=True)

    saved_copy = dictionary.copy()
    if len(dictionary) > 1:
        while len(dictionary) - 1:
            last, previous, dictionary = dictionary[-1], dictionary[-2], dictionary[:-2]
            dictionary += [(Node(last[0], previous[0]), last[1] + previous[1])]
            dictionary = sorted(dictionary, key=lambda x: x[1], reverse=True)

        codes = huffman(dictionary[0][0])
    else:
        codes = {dictionary[0][0]: "0"}

    outputFile = open(argv[3], "wb")

    outputFile.write(str(sum([len(codes[saved_copy[i][0]]) * saved_copy[i][1] for i in range(len(saved_copy))])).encode())
    outputFile.write(chr(128).encode())
    outputFile.write(str(len(codes.keys())).encode())
    outputFile.write(chr(128).encode())
    for symbol in list(map(lambda x: x[0], saved_copy)):
        outputFile.write(f"{symbol}{chr(len(codes[symbol]))}{chr(btoi(codes[symbol]))}".encode())

    buffer = ""

    for symbol in open(argv[2], "rb").read():
        symbol = chr(symbol)
        buffer += codes[symbol]
        if len(buffer) >= 7:
            outputFile.write(chr(btoi(buffer[:7])).encode())
            buffer = buffer[7:]
    if buffer != "":
        buffer += "0" * (8 - len(buffer))
    outputFile.write(chr(btoi(buffer[:7])).encode())

    outputFile.close()

    endTime = datetime.now().second * (10 ** 6) + datetime.now().microsecond

    print(f"Finished in {endTime - startTime}μss. Coef is {len(open(argv[2], 'rb').read()) / len(open(argv[3], 'rb').read())}")

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
    
    if len(argv) < 4:
        print("Not enough arguments")
        exit(1)

    startTime = datetime.now().second * (10 ** 6) + datetime.now().microsecond

    data = open(argv[2], "rb").read().decode().split(chr(128))
    file = open(argv[3], "w")
    number = int(data[0])
    groups = int(data[1])
    data = chr(128).join(data[2:])
    codes, data = data[:groups * 3], data[groups * 3:]

    # for i in range(0, len(codes), 3):
    #     print(f"{codes[i]}: {itob(ord(codes[i + 1]), ord(codes[i + 2]))}")
    codes = {(codes[i]): itob(ord(codes[i + 1]), ord(codes[i + 2])) for i in range(0, len(codes), 3)}
    # exit(0)

    data = "".join([itob(7, ord(data[i])) for i in range(len(data))])

    while data != "" and number != 0:
        for symbol in codes.keys():
            if data.startswith(codes[symbol]):
                file.write(symbol)
                data = data[len(codes[symbol]):]
                number -= len(codes[symbol])
                break
        
    file.close()
    endTime = datetime.now().second * (10 ** 6) + datetime.now().microsecond

    print(f"Finished in {endTime - startTime}μss.")

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
