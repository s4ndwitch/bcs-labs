from sys import argv

def encode():

    if len(argv) < 4:
        return

    data = open(argv[2], "r").read().strip()
    file = open(argv[3], "w")

    codes = [chr(i) for i in range(256)]

    line = data[0]
    for symbol in data[1:]:
        if ((line + symbol) in codes):
            line += symbol
        else:
            file.write(chr(codes.index(line)))
            codes += [line + symbol]
            line = symbol
    file.write(chr(codes.index(line)))
    file.close()

def decode():
    
    if len(argv) < 4:
        return

    data = open(argv[2], "r").read().strip()
    file = open(argv[3], "w")

    codes = [chr(i) for i in range(256)]

    code = data[0]
    file.write(code)
    symbol = code
    for ncode in data[1:]:
        if (ord(ncode) >= len(codes)):
            line = codes[ord(code)]
            line += symbol
        else:
            line = codes[ord(ncode)]
        file.write(line)
        symbol = line[0]
        codes += [codes[ord(code)] + symbol]
        code = ncode
    file.close()

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