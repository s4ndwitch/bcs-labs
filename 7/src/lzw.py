#!/usr/bin/python3

from sys import argv
from datetime import datetime

def encode():

    if len(argv) < 4:
        return
    
    startTime = datetime.now().second * (10 ** 6) + datetime.now().microsecond

    data = open(argv[2], "rb").read().strip()
    file = open(argv[3], "w")

    codes = [chr(i) for i in range(256)]

    line = chr(data[0])
    for symbol in data[1:]:
        symbol = chr(symbol)
        if ((line + symbol) in codes):
            line += symbol
        else:
            file.write(chr(codes.index(line)))
            codes += [line + symbol]
            line = symbol
    file.write(chr(codes.index(line)))
    file.close()

    endTime = datetime.now().second * (10 ** 6) + datetime.now().microsecond

    print(f"Finished in {endTime - startTime}μss. Coef is {len(open(argv[2], 'rb').read()) / len(open(argv[3], 'rb').read())}")

def decode():
    
    if len(argv) < 4:
        return
    
    startTime = datetime.now().second * (10 ** 6) + datetime.now().microsecond

    data = open(argv[2], "r").read().strip()
    file = open(argv[3], "w")

    codes = [chr(i) for i in range(256)]

    code = data[0]
    file.write(code)
    symbol = code
    for ncode in data[1:]:
        if (ord(ncode) >= len(codes)):
            if len(codes) < 1114111:
                line = codes[ord(code)]
                line += symbol
        else:
            line = codes[ord(ncode)]
        file.write(line)
        symbol = line[0]
        codes += [codes[ord(code)] + symbol]
        code = ncode
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
