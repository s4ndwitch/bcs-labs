
#!/bin/python3

from sys import argv;
from random import randint;
from base64 import b64encode, b64decode


class LengthError(Exception):
    def __init__(self: object) -> None:
        super().__init__("Not enough arguments")


def gcd(a: int, b: int) -> int:
    while (a > 0 and b > 0):
        if a > b:
            a -= b;
        else:
            b -= a;
    return a if a > 0 else b;

def finish(a: str) -> str:
    while len(a) < 8:
        a = "0" + a;
    return a;

def generate() -> None:

    counter: int = 0;

    key: int = []
    for i in range(8):
        key += [randint(counter + 1, counter + 11)];
        counter += key[-1];
    
    counter: int = randint(counter + 1, counter + 11);
    n: int = 1
    for i in range(2, counter):
        if gcd(i, counter) == 1:
            n: int = i;
            break;
    
    print("Where to save public key? (mh.pub): ", end="");
    file_name: str = input();
    open(file_name if file_name.strip() != "" else "mh.pub", "w").write(
        "/".join(map(lambda x: b64encode(hex(int((x * n) % counter))[2:].encode()).decode(), key))
    );

    for i in range(1, 2 ** int.__sizeof__(1), counter):
        if i % n == 0:
            n = (int)(i / n);
            break;
    
    print("Where to save private key? (mh.priv): ", end="");
    file_name: str = input();
    open(file_name if file_name.strip() != "" else "mh.priv", "w").write(
        "/".join(map(lambda x: b64encode(hex(x)[2:].encode()).decode(), key)) \
            + f"/{b64encode(hex(n)[2:].encode()).decode()}/{b64encode(hex(counter)[2:].encode()).decode()}"
    );

    return;

def encode() -> None:

    if len(argv) < 5:
        raise LengthError;

    key = list(map(lambda x: int(b64decode(x).decode(), base=16),
                    open(argv[3], "r").read().strip().split("/")));

    with open(argv[4], "w") as file:
        for symbol in open(argv[2], "r").read():
            code = finish(bin(ord(symbol))[2:])
            count = 0
            for i in range(8):
                count += int(code[i]) * key[i]
            file.write(b64encode(hex(count)[2:].encode()).decode() + "/")

def decode() -> None:

    if len(argv) < 5:
        raise LengthError;

    key = list(map(lambda x: int(b64decode(x).decode(), base=16),
                    open(argv[3], "r").read().strip().split("/")));
    key, n, m = key[:8], key[8], key[9];

    with open(argv[4], "w") as file:
        for byte in open(argv[2], "r").read().strip().split("/")[:-1]:
            number = (int(b64decode(byte).decode(), base=16) * n) % m;
            symbol = 0
            for i in range(7, -1, -1):
                if key[i] <= number:
                    symbol += 2 ** (7 - i)
                    number -= key[i]
            file.write(chr(symbol));

    return;

if __name__ == "__main__":
    if len(argv) < 2:
        raise LengthError;
    if argv[1] == "-g" or argv[1] == "--generate":
        generate();
    elif argv[1] == "-d" or argv[1] == "--decode":
        decode();
    elif argv[1] == "-e" or argv[1] == "--encode":
        encode();
