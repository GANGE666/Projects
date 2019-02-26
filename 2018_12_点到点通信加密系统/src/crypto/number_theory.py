# coding=utf-8

import random


# gcd
def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)


# 扩展gcd
def extendedGCD(a, b):
    # a*xi + b*yi = ri
    if b == 0:
        return 1, 0, a
    # a*x1 + b*y1 = a
    x1 = 1
    y1 = 0
    # a*x2 + b*y2 = b
    x2 = 0
    y2 = 1
    while b != 0:
        q = int(a / b)
        # ri = r(i-2) % r(i-1)
        r = a % b
        a = b
        b = r
        # xi = x(i-2) - q*x(i-1)
        x = x1 - q*x2
        x1 = x2
        x2 = x
        # yi = y(i-2) - q*y(i-1)
        y = y1 - q*y2
        y1 = y2
        y2 = y
    return x1, y1, a


# 快速幂
def fastExpMod(b, e, m):
    result = 1
    while e != 0:
        if (e & 1) == 1:
            # ei = 1, then mul
            result = (result * b) % m
        e >>= 1
        # b, b^2, b^4, b^8, ... , b^(2^n)
        b = (b*b) % m
    return result


# 欧拉函数
def euler(n):
    res = n
    a = n
    i = 2
    while i*i <= a:
        if a % i == 0:
            res = int(res/i)*(i-1)
            while a % i == 0:
                a = int(a/i)

        i += 1

    # print("%d\t%d"%(int(res/a),a-1))

    if a > 1:
        return int(res/a)+1, (a-1)+1
        # res = int(res/a) * (a-1)
    return res


# 素性检验
def primeTest(n):
    q = n - 1
    k = 0
    # Find k, q, satisfied 2^k * q = n - 1
    while q % 2 == 0:
        k += 1
        q = int(q/2)
    a = random.randint(2, n-2)
    # If a^q mod n= 1, n maybe is a prime number
    if fastExpMod(a, q, n) == 1:
        return True
    # If there exists j satisfy a ^ ((2 ^ j) * q) mod n == n-1, n maybe is a prime number
    for j in range(0, k):
        if fastExpMod(a, (2**j)*q, n) == n - 1:
            return True
    # a is not a prime number
    return False


# 生成素数
def findPrime(halfkeyLength):
    while True:
        # Select a random number n
        n = random.randint(0, 1 << halfkeyLength)
        if n % 2 != 0:
            found = True
            print("testing %d" % n)
            # If n satisfy primeTest 10 times, then n should be a prime number
            for i in range(0, 10):
                if not primeTest(n):
                    found = False
                    break
            if found:
                return n


if __name__ == "__main__":
    findPrime(256)



