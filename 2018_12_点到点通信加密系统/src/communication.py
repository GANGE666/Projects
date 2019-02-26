# coding=utf-8


import keymanager
import crypto.MsgCrypto
import network
import os
import time
import threading
import json

# 首部类型字段
msg_type_dict = {
    'TRANSFER_STRING': (10).to_bytes(4, byteorder='little'),
    'TRANSFER_FILE': (11).to_bytes(4, byteorder='little'),
    'UPDATE_SM4_KEY': (20).to_bytes(4, byteorder='little'),
}


class Communication:
    def __init__(self, mode='LISTEN', target_ip='127.0.0.1', port=2333, msg_queue=[]):
        # 显示信息队列
        self.msg_queue = msg_queue

        #初始化接收方信息
        if mode == 'LISTEN':
            self.target_config = {"target_ip": '127.0.0.1', "port": 2333}
        else:
            self.fetch_local_config()

        # 初始化模块
        self.crypt = crypto.MsgCrypto.MsgCrypto()
        self.sm4_key = keymanager.KeyManager().get_sm4_key()
        self.km = keymanager.KeyManager()

        # 双方rsa初始化
        self.km.fetch_local_key()
        self.crypt.rsa_my_init(n=self.km.get_rsa_n(), e=self.km.get_rsa_private_key(), d=self.km.get_rsa_public_key())
        self.crypt.rsa_target_init(n=self.km.get_rsa_n(), e=self.km.get_rsa_private_key(), d=self.km.get_rsa_public_key())

        # 用keymanager取到的sm4密钥设置crypt模块
        self.crypt.sm4_init(key=self.sm4_key)

        # 监听端口或连接
        self.trans = network.MySocket(msg_queue=self.msg_queue)
        if mode == 'LISTEN':
            self.trans.listen(port)
        elif mode == 'NONE':
            pass
        else:
            self.trans.connect(self.target_config["target_ip"], self.target_config["port"])
            # 协商sm4_key
            self.send_sm4_key(self.sm4_key)

    ''' LISTEN '''
    # 监听的管理程序，判断收到的包类型，调用相应的函数处理发送来的数据
    def listen_manager(self):
        while True:
            # 接受type码
            msg_type = self.trans.recv_bytes(4)

            if msg_type in msg_type_dict.values():
                if msg_type == msg_type_dict['TRANSFER_STRING']:
                    string = self.recv_string()
                    self.msg_queue.append("收到string:\n%s\n" % string)

                elif msg_type == msg_type_dict['TRANSFER_FILE']:
                    f_name = self.recv_file()
                    if f_name:
                        self.msg_queue.append("收到文件:\n储存到%s\n" % f_name)
                    else:
                        self.msg_queue.append("接收文件通信过程中出现差错，请重试\n")

                elif msg_type == msg_type_dict['UPDATE_SM4_KEY']:
                    if self.recv_sm4_key():
                        # print("Update sm4 success!")
                        self.msg_queue.append("更新sm4 key成功\n")
                    else:
                        print("Update sm4 fail!")
                        self.msg_queue.append("更新sm4 key失败\n")
        pass

    # 接收字符串
    def recv_string(self):
        print("recv string now")
        # 接收 加密数据长度、原数据hash
        str_encry_len = int.from_bytes(self.trans.recv_bytes(size=4), byteorder='little')
        recv_hash = hex(int.from_bytes(self.trans.recv_bytes(size=32), byteorder='little')).replace("0x", "")
        # 接收加密数据
        str_encry = self.trans.recv_bytes(str_encry_len)

        # 解密数据
        string = self.crypt.sm4_decrypt_msg(str_encry)

        # 计算解密数据的hash
        calc_hash = self.crypt.sm3_hash_str(string)

        # print("recv_string: " + string)

        # 计算得到的hash与接收到的做对比
        if recv_hash == calc_hash:
            return string
        else:
            return None

    # 接收文件
    def recv_file(self):
        print("recv file now")

        file_encry_len = int.from_bytes(self.trans.recv_bytes(size=4), byteorder='little')
        recv_hash = hex(int.from_bytes(self.trans.recv_bytes(size=32), byteorder='little')).replace("0x", "")

        # 接收文件，得到临时文件名
        f_temp_name = self.trans.recv_file(file_encry_len)

        # 接收文件成功
        if f_temp_name:
            # 解密文件
            f_path = self.crypt.sm4_decrypt_file(f_temp_name, mode='network')

            # 计算解密文件的hash
            calc_hash = self.crypt.sm3_hash_file(f_path)

            if recv_hash == calc_hash:
                print("File comp Succ! 解密文件与传输来得hash值相同\n")
                return f_path
            else:
                print("File comp Fail! 解密文件与传输来得hash值不同，请重新传输")
                return None
        else:
            print("Recv fail!")
            return None

    # 更新sm4 key
    def recv_sm4_key(self):
        print("recv sm4key now")
        self.msg_queue.append("正在更新会话sm4 key\n")

        sm4_key_len = int.from_bytes(self.trans.recv_bytes(size=4), byteorder='little')
        recv_hash = hex(int.from_bytes(self.trans.recv_bytes(size=32), byteorder='little')).replace("0x", "")
        # 接收加密数据
        sm4_key_encry_bytes = self.trans.recv_bytes(sm4_key_len)

        # 还原成数字
        sm4_key_encry = int.from_bytes(sm4_key_encry_bytes, byteorder='big')
        # 解密数据
        sm4_key = self.crypt.rsa_decrypt(sm4_key_encry)

        # 计算解密后的hash
        calc_hash = self.crypt.sm3_hash_str(str(sm4_key))

        if calc_hash == recv_hash:
            self.crypt.sm4_init(sm4_key)
            print("Update sm4 key succ")
            print("\t本次会话得sm4 key 为: %x\n" % sm4_key)
            self.msg_queue.append("本次会话得sm4 key 为: %x\n" % sm4_key)
            return True
        else:
            print("Update sm4 key fail")
            self.msg_queue.append("[ERROR] 未验证公钥\n")
            return False

    ''' CONNECT '''
    # 发送一个字符串
    def send_string(self, string):

        str_encry = self.crypt.sm4_encrypt_msg(string)
        str_encry_len = len(str_encry)

        # 按照协议发送
        # 数据类型(4) ---- 加密后的数据长度(4) ---- hash(32) ---- 加密数据(n)
        self.trans.send_bytes(msg_type_dict['TRANSFER_STRING'])
        self.trans.send_bytes(str_encry_len.to_bytes(4, byteorder='little'))
        self.trans.send_bytes(int(self.crypt.sm3_hash_str(string), base=16).to_bytes(32, byteorder='little'))

        self.trans.send_bytes(str_encry)

        return

    # 发送文件
    def send_file(self, f_path):

        f_path_encry = f_path+".encrypt"
        self.crypt.sm4_encrypt_file(f_path, mode='network')
        file_encry_size = os.path.getsize(f_path_encry)

        self.trans.send_bytes(msg_type_dict['TRANSFER_FILE'])
        self.trans.send_bytes(file_encry_size.to_bytes(4, byteorder='little'))
        self.trans.send_bytes(int(self.crypt.sm3_hash_file(f_path), base=16).to_bytes(32, byteorder='little'))

        # 传入文件名
        self.trans.send_file(f_path)

        print("send file succ!\n")

        return

    # 协商sm4 key
    def send_sm4_key(self, sm4_key):
        print("正在协商sm4 key")
        self.msg_queue.append("正在协商sm4 key\n")
        # 传入列表 or 数字 形式的sm4_key
        if isinstance(sm4_key, list):
            sm4_key = int.from_bytes(bytes(sm4_key), byteorder='big')

        # 使用对方公钥加密 TODO: 对方公钥加密
        sm4_key_encry_by_pubilckey = self.crypt.rsa_encrypt(sm4_key)

        sm4_key_encry_by_pubilckey_bytes = sm4_key_encry_by_pubilckey.to_bytes(128, byteorder='big')
        sm4_key_encry_len = len(sm4_key_encry_by_pubilckey_bytes)

        # 发送
        self.trans.send_bytes(msg_type_dict['UPDATE_SM4_KEY'])
        self.trans.send_bytes(sm4_key_encry_len.to_bytes(4, byteorder='little'))
        self.trans.send_bytes(int(self.crypt.sm3_hash_str(str(sm4_key)), base=16).to_bytes(32, byteorder='little'))

        self.trans.send_bytes(sm4_key_encry_by_pubilckey_bytes)
        print("send key succ!")
        print("发送sm4 key成功\n本次会话的sm4为: %x\n" % sm4_key)
        self.msg_queue.append("发送sm4 key成功\n本次会话的sm4为: %x\n" % sm4_key)
        return

    # 加载接收方的本地配置信息
    def fetch_local_config(self):
        try:
            with open("target_config.json", "r") as f:
                self.target_config = json.load(f)
        except:
            print("target_config.json not exist or broken")

    # 保存接收端的配置信息到本地文件
    def dump_local_config(self):
        with open("target_config.json", "w") as f:
            json.dump(self.target_config, f)
            print("[*] dump target_config succ!")


def test_listen():
    print("test_listen begin")
    co = Communication(mode='LISTEN')
    time.sleep(3)
    co.listen_manager()

def test_conn():
    print("test_conn begin")
    co = Communication(mode='CONNECT')
    co.send_string("hello!")
    # co.send_file("1.txt")
    # co.send_sm4_key(co.km.get_sm4_key())

if __name__ == "__main__":
    co = Communication(mode='NONE')
    co.dump_local_config()

    t1 = threading.Thread(target=test_listen)
    t2 = threading.Thread(target=test_conn)

    t1.start()
    time.sleep(1)
    t2.start()
