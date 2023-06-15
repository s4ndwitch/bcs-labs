

from sys import argv
from random import randint

def read_bytes(file_name: str) -> int:
	return list(map(lambda x: int(x, 16), open(file_name, "r").read().strip().split("/")))

def write_bytes(file_name: str, data: int) -> int:
	open(file_name, "w").write("/".join(list(map(lambda x: hex(x)[2:], data))))
	return 0

#################### Encoding #######################

def cypher(key: int, symbol: int) -> int:

	result: int = 0
	for i in range(1, 9):
		if (symbol % 2 == 1):
			result += key[-i]
		symbol //= 2
	return result

def encode():
	return 1 if len(argv) < 5 else write_bytes(argv[4], list(map(lambda x: cypher(read_bytes(argv[3]), ord(x)), open(argv[2], "r").read().strip())))


############### Decoding #####################

def decypher(key: int, nr: int, m: int, symbol: int) -> int:
	code: int = (symbol * nr) % m
	result: int = 0
	for i in range(1, 9):
		if key[-i] <= code:
			code -= key[-i]
			result += 2 ** (i - 1)
	return result

def decode():
	if len(argv) < 5:
		return 1

	open(argv[4], "w").write("".join(list(map(lambda x: chr(decypher(read_bytes(argv[3])[:-2], read_bytes(argv[3])[-1], read_bytes(argv[3])[-2], x)), read_bytes(argv[2])))))

	return 0


################### Generating ####################

def euqlid(a: int, b: int) -> int:
	while a != 0 and b != 0:
		if a > b:
			a -= b
		else:
			b -= a
	return a if a != 0 else b

def generate():

	pub_name = "mh.pub"
	priv_name = "mh.priv"

	if len(argv) > 3:
		pub_name = argv[2]
		priv_name = argv[3]

	priv_key = []
	for _ in range(8):
		priv_key += [sum(priv_key) + randint(1, 1000)]
	m = sum(priv_key) + randint(1, 1000)

	n = m - 1
	for i in range(2, m):
		if euqlid(i, m) == 1:
			n = i
			break

	nr = 1
	while nr % n != 0:
		nr += m
	nr //= n

	pub_key = list(map(lambda x: (x * n) % m, priv_key))
	write_bytes(pub_name, pub_key)
	write_bytes(priv_name, priv_key + [m] + [nr])

	return 0


################### main #######################

if __name__ == "__main__":

	if len(argv) < 2:
		raise IndexError("Not enough arguments")
		exit(1)

	if argv[1] in ["-d", "--decode"]:
		exit(decode())
	elif argv[1] in ["-e", "--encode"]:
		exit(encode())
	elif argv[1] in ["-g", "--generate"]:
		exit(generate())
	else:
		raise ValueError("Unknown flag on the first position")
		exit(2)

	exit(0)
