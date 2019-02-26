# coding=utf-8


import struct
import time
import config_util


# Usage:     print(Sm3().hash_msg("abc"))

class Sm3:

    IV = [0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e]
    T_j = [
            0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519,
            0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a,
            0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a,
            0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a,
        ]

    def __int__(self):
        pass

    def rotate_left(self, a, k):
            k = k % 32
            return ((a << k) & 0xFFFFFFFF) | ((a & 0xFFFFFFFF) >> (32 - k))

    def FF_j(self, X, Y, Z, j):
            if 0 <= j and j < 16:
                    ret = X ^ Y ^ Z
            elif 16 <= j and j < 64:
                    ret = (X & Y) | (X & Z) | (Y & Z)
            return ret

    def GG_j(self, X, Y, Z, j):
            if 0 <= j and j < 16:
                    ret = X ^ Y ^ Z
            elif 16 <= j and j < 64:
                    #ret = (X | Y) & ((2 ** 32 - 1 - X) | Z)
                    ret = (X & Y) | ((~ X) & Z)
            return ret

    def P_0(self, X):
            return X ^ (self.rotate_left(X, 9)) ^ (self.rotate_left(X, 17))

    def P_1(self, X):
            return X ^ (self.rotate_left(X, 15)) ^ (self.rotate_left(X, 23))

    def CF(self, V_i, B_i):
            W = []
            for j in range(0, 16):
                    W.append(0)
                    unpack_list = struct.unpack(">I", B_i[j*4:(j+1)*4])
                    W[j] = unpack_list[0]
            for j in range(16, 68):
                    W.append(0)
                    W[j] = self.P_1(W[j-16] ^ W[j-9] ^ (self.rotate_left(W[j-3], 15))) ^ (self.rotate_left(W[j-13], 7)) ^ W[j-6]
                    # str1 = "%08x" % W[j]
            W_1 = []
            for j in range(0, 64):
                    W_1.append(0)
                    W_1[j] = W[j] ^ W[j+4]
                    # str1 = "%08x" % W_1[j]

            A, B, C, D, E, F, G, H = V_i
            """
            print "00",
            out_hex([A, B, C, D, E, F, G, H])
            """
            for j in range(0, 64):
                    SS1 = self.rotate_left(((self.rotate_left(A, 12)) + E + (self.rotate_left(self.T_j[j], j))) & 0xFFFFFFFF, 7)
                    SS2 = SS1 ^ (self.rotate_left(A, 12))
                    TT1 = (self.FF_j(A, B, C, j) + D + SS2 + W_1[j]) & 0xFFFFFFFF
                    TT2 = (self.GG_j(E, F, G, j) + H + SS1 + W[j]) & 0xFFFFFFFF
                    D = C
                    C = self.rotate_left(B, 9)
                    B = A
                    A = TT1
                    H = G
                    G = self.rotate_left(F, 19)
                    F = E
                    E = self.P_0(TT2)

                    A &= 0xFFFFFFFF
                    B &= 0xFFFFFFFF
                    C &= 0xFFFFFFFF
                    D &= 0xFFFFFFFF
                    E &= 0xFFFFFFFF
                    F &= 0xFFFFFFFF
                    G &= 0xFFFFFFFF
                    H &= 0xFFFFFFFF

            V_i_1 = []
            V_i_1.append(A ^ V_i[0])
            V_i_1.append(B ^ V_i[1])
            V_i_1.append(C ^ V_i[2])
            V_i_1.append(D ^ V_i[3])
            V_i_1.append(E ^ V_i[4])
            V_i_1.append(F ^ V_i[5])
            V_i_1.append(G ^ V_i[6])
            V_i_1.append(H ^ V_i[7])
            return V_i_1

    # 输入 字节流
    def hash_msg(self, msg_bytes):
        len1 = len(msg_bytes)
        reserve1 = len1 % 64
        msg1 = msg_bytes + struct.pack("B",128)
        reserve1 = reserve1 + 1
        for i in range(reserve1, 56):
                msg1 = msg1 + struct.pack("B",0)


        bit_length = (len1) * 8
        bit_length_string = struct.pack(">Q", bit_length)
        msg1 = msg1 + bit_length_string

        group_count = int(len(msg1) / 64 )

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            begin_time = time.time()*1000
            print("开始时间：%d" % begin_time)

        B = []
        for i in range(0, group_count):
                B.append(0)
                B[i] = msg1[i*64:(i+1)*64]

        V = []
        V.append(0)
        V[0] = self.IV
        for i in range(0, group_count):
                V.append(0)
                V[i+1] = self.CF(V[i], B[i])

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            end_time = time.time()*1000
            print("结束时间：%d" % end_time)
            print("SM3 用时:" + str(end_time - begin_time) + "毫秒")

        return "".join('%08x' % i for i in V[i+1])



if __name__ == "__main__":

    print("abc")
    y = Sm3().hash_msg("abc".encode('utf-8'))
    print("result: ")
    print(y)

 
"""
66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
"""


