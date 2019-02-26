# coding=utf-8

import crypto.RSA
import crypto.SM4
import crypto.SM3
import os
import config_util


class MsgCrypto:
    def __init__(self):
        # 一个是自己的rsa 一个是对方的rsa
        self.my_rsa = None
        self.target_rsa = None
        self.sm4 = None
        self.sm3 = crypto.SM3
        print("[*] \tRemember to init rsa and sm4")
        return

    # 发送方rsa初始化
    def rsa_my_init(self, p=0, q=0, n=0, e=0, d=0):
        self.my_rsa = crypto.RSA.RSA(p=p, q=q, n=n, e=e, d=d, mode='NETWORK')
        self.my_rsa.print_detail()
        return

    # 接收方rsa初始化
    def rsa_target_init(self, p=0, q=0, n=0, e=0, d=0):
        self.target_rsa = crypto.RSA.RSA(p=p, q=q, n=n, e=e, d=d, mode='NETWORK')
        return

    # sm4 初始化
    def sm4_init(self, key, mode=crypto.SM4.ENCRYPT):
        self.sm4 = crypto.SM4.Sm4()
        if isinstance(key, list):
            self.sm4.sm4_set_key(key, mode)
        else:
            # print(hex(key))
            key_list = list(key.to_bytes(16, byteorder='big'))
            self.sm4.sm4_set_key(key_list, mode)

    # 用对方公钥加密
    def rsa_encrypt(self, m):
        return self.target_rsa.encrypt(m)

    # 用自己的私钥解密
    def rsa_decrypt(self, c):
        return self.my_rsa.decrypt(c)

    def sm4_encrypt_msg(self, m):
        # sm4 加密字符串
        # 输入 字符串
        # 输出 bytes
        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("SM4 加密字符串:%s" % m)
            print("utf-8编码后得明文:")
            print(m.encode("utf-8"))

        c = bytes(self.sm4.sm4_crypt_ecb(list(m.encode("utf-8")), mode=crypto.SM4.ENCRYPT))

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("密文为:")
            print(c)

        return c

    def sm4_decrypt_msg(self, c):
        # sm4 解密bytes
        # 输入 bytes
        # 输出 字符串
        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("收到密文为:")
            print(c)

        m = str(bytes(self.sm4.sm4_crypt_ecb(list(c), mode=crypto.SM4.DECRYPT)), encoding="utf-8")

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("SM4 解密且utf-8编码后得到字符串:%s" % m)

        return m

    # sm4 加密文件 正常模式不包括文件名，network模式在文件开头添加f_name+'\0'
    def sm4_encrypt_file(self, f_path, mode='normal'):
        # sm4 加密文件
        # 输入 文件名
        # 输出 加密文件
        with open(f_path, "rb") as f:
            file_content = f.read()
        file_content_list = []

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("正在加密文件：%s" % f_path)

        # 网络模式则将文件名放在首位
        if mode == 'network':
            f_name = os.path.basename(f_path)
            file_content_list = list(f_name.encode('utf-8'))
            file_content_list.append(0)

        file_content_list.extend(list(file_content))

        c = bytes(self.sm4.sm4_crypt_ecb(file_content_list, mode=crypto.SM4.ENCRYPT))

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("文件加密成功，加密文件暂存为：%s" % f_path+".encrypt")

        with open(f_path+".encrypt", "wb") as f:
            f.write(c)

        return True

    # sm4 解密文件
    def sm4_decrypt_file(self, f_path=None, mode='normal'):
        # sm4 解密文件
        # 输入 加密文件
        # 输出 文件
        with open(f_path+".encrypt", "rb") as f:
            file_content = f.read()
        file_content_list = list(file_content)

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("解密文件：%s" % f_path+".encrypt")

        file_content = self.sm4.sm4_crypt_ecb(file_content_list, mode=crypto.SM4.DECRYPT)

        if mode == 'network':
            f_name = []
            i = 0
            while file_content[i] != 0:
                f_name.append(file_content[i])
                i += 1
            file_content_bytes = bytes(file_content[i+1:])
            f_path = "recv/"+str(bytes(f_name), encoding='utf-8')
        else:
            file_content_bytes = bytes(file_content)

        with open(f_path, "wb") as f:
            f.write(file_content_bytes)

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("解密得到文件：%s，已存放到recv文件夹下，即将进行hash检验" % f_path)

        return f_path

    def sm3_hash_str(self, msg):
        # sm3 hash字符串
        # 输入 字符串
        # 输出 16进制字符串
        return self.sm3.Sm3().hash_msg(msg.encode("utf-8"))

    def sm3_hash_file(self, f_path):
        # sm3 hash文件
        # 输入 文件路径
        # 输出 16进制字符串
        with open(f_path, "rb") as f:
            file_content = f.read()

        if config_util.CRYPTO_MODE_DEBUG_OUTPUT:
            print("正在对%s 进行SM3：" % f_path)
            print("文件大小：%d" % len(file_content))

            file_hash = self.sm3.Sm3().hash_msg(file_content)

            print("hash值为：%s" % file_hash)

        return file_hash



