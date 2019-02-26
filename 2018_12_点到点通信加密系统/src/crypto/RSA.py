# coding=utf-8

#  RSA 加解密
from crypto.number_theory import *
import random

# 只能加密数字
class RSA:

    def random_select_p_q(self):
        # 随机生成p q
        print("[*] Initializing RSA!")
        self.p = findPrime(int(self.keylen/2))
        self.q = findPrime(int(self.keylen/2))

    def select_e(self):
        # 随机产生e
        while True:
            e = random.randint(0, 1 << self.keylen)
            if gcd(e, self.fn) == 1:
                self.e = e
                return

    # 输入：
    # (p q)|(n) + (e)|(d)
    def __init__(self, p=0, q=0, n=0, e=0, d=0, keylen=0, mode='SMALL'):

        self.p = p
        self.q = q

        self.n = n
        self.fn = 0

        self.keylen = keylen

        if self.n == 0:
            if p == 0 and q == 0:
                self.random_select_p_q()

            self.calc_n()
            self.calc_fn()

        elif mode == 'SMALL':
            self.calc_fn()

        if e == 0 and d == 0:
            self.select_e()
            self.calc_d_from_e()
        else:
            self.e = e
            self.d = d
            if mode == 'SMALL':
                if e == 0:
                    self.calc_e_from_d()
                else:
                    self.calc_d_from_e()

    def calc_n(self):
        self.n = self.p * self.q
        return

    def calc_fn(self):
        # 要求p q都为素数
        if self.p == 0 and self.q == 0:
            self.p, self.q = euler(self.n)
        self.fn = (self.p-1)*(self.q-1)
        return

    def calc_e_from_d(self):
        x, y, r = extendedGCD(self.fn, self.d)

        if y < 0:
            y += self.fn

        self.e = int(y)

    def calc_d_from_e(self):
        x, y, r = extendedGCD(self.fn, self.e)

        if y < 0:
            y += self.fn

        self.d = int(y)

    def encrypt(self, m):
        return fastExpMod(m, self.e, self.n)

    def decrypt(self, c):
        return fastExpMod(c, self.d, self.n)

    def print_detail(self):
        print("本机RSA信息：")
        print("p:\t%d" % self.p)
        print("q:\t%d" % self.q)
        print("n=p*q:\t%d" % self.n)
        print("fn=(p-1)*(q-1)\t%d" % self.fn)
        print("e:\t%d" % self.e)
        print("d:\t%d" % self.d)


if __name__ == "__main__":

    rsa = RSA(p=113129257426828281432524187580381047997575559435912947226758623539913650449103,
              q=72411587307657685190012970300333056897718862529304585414536106974544325602667,
              keylen=512,
              #e=690168106656618040363564980207720808173548644742090959344731597233406438873659640773006169470585788386185706975041358242806911670242192040328957861436269,
              #d=6345123150999433290055459257694242694285757852953109410409097639278332519713287523014246876590980410002685259388087970737294629981941700870641726541762841,
              )
    rsa.print_detail()

    key = 218704240496475236270021651107770293820
    print(key)
    c = rsa.encrypt(key)
    m = rsa.decrypt(c)
    print(c)
    print(m)

    for i in range(1000):
        k = random.randint(0, 1<<511)
        c = rsa.encrypt(k)
        m = rsa.decrypt(c)
        print(k)
        print(c)
        print(m)
        print()
        if k != m:
            print("unequ")
            break
