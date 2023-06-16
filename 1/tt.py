def euqlid(a: int, b: int) -> int:
    while a != 0 and b != 0:
        if a > b:
            a -= b
        else:
            b -= a
    return a if a != 0 else b

def miller(N: int) -> int:

    count = 0

    a = 1
    for a in range(2, N):
        count += 1
        if euqlid(a, N) == 1:
            break

    if a == 1:
        return 1

    s = 0
    for i in range(1, N):
        if 2 ** s > N - 1:
            s = 0
            break
        count += 1
        if (N - 1) % (2 ** s) == 0:
            while ((N - 1) % (2 ** (s + 1))) == 0:
                count += 1
                s += 1
            break

    t = (N - 1) // (2 ** s)
    count += 1
    print(f"N-1 = (2^{s})*{t}")
    print(f"GCD(N, {a}) = 1")

    for i in range(s):
        count += 2
        if (a ** ((2 ** s) * t) - 1) % N == 0:
            print(f"(a^((2^{s})*t)-1)(mod N) = 0")
            print(f"count = {count}")
            return 1
        if (a ** ((2 ** s) * t) + 1) % N == 0:
            print("(a^((2^{s})*t)+1)(mod N) = 0")
            print(f"count = {count}")
            return 1
    
    print(f"count = {count}")
    return 0
