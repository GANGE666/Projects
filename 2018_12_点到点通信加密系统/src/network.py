# coding=utf-8


'''
TODO:
文件传输 传输size content 校验 确认
'''
import socket
import threading
import traceback
import time

class MySocket:

    LIESTEN = 0
    CONNECT = 1

    def __init__(self, msg_queue=[]):
        self.msg_queue = msg_queue

        self.sk = socket.socket()
        self.counter = 0
        pass

    # 接收新的连接
    def accept_new_conn(self):
        while True:
            self.conn, self.address = self.sk.accept()
            t = threading.Thread(target=self.accept_new_conn)
            t.start()
            self.msg_queue.append("收到新的连接！\n")
            return

    def listen(self, port=2333):
        try:
            self.sk.bind(("127.0.0.1", port))
            self.sk.listen(5)
            self.msg_queue.append("[*] 监听端口%d成功，等待连接\n" % port)
            print("[*] 监听端口%d成功，等待连接\n" % port)

            # self.conn, self.address = self.sk.accept()
            self.accept_new_conn()
        except Exception as e:
            print("[ERROR] \tSocket bind fail!")
            print(e)
            self.msg_queue.append("[ERROR] %d 端口监听失败！\n请检查端口是否已经被占用\ncmd: netstat -ano|findstr \"%d\"\n" % (port, port))
            exit()
        return

    def recv_bytes(self, size=1024):
        try:
            res = self.conn.recv(size)
            return res
        except Exception as e:
            print("[WARING] \trecv fail!")
            print(e)
        return None

    def recv_str(self, size=1024):
        try:
            res = self.conn.recv(size)
            return str(res, encoding="utf-8")
        except Exception as e:
            print("[WARING] \trecv string fail!")
            print(e)
        return None

    # 比较特殊，返回值为临时文件名
    def recv_file(self, size):
        try:
            content = self.conn.recv(size)
            temp_f_name = "recv/"+str(time.time()*10000 + self.counter)
            self.counter += 1

            with open(temp_f_name+".encrypt", "wb") as f:
                f.write(content)
            return temp_f_name
        except Exception as e:
            print("[WARING] \trecv file fail!")
            print(e)
            return None

    '''分界线 下面是发送方'''

    def connect(self, ip="127.0.0.1", port=2333):
        try:
            self.sk.connect((ip, port))
            print("[INFO] \tSocket connect succ!")
            self.msg_queue.append("[*] 成功连接至%s : %d \n" % (ip, port))
        except Exception as e:
            print("[ERROR] \tSocket connect fail!")
            print(e)
            self.msg_queue.append("[ERROR] %s : %d 建立连接失败！\n请检查目标主机是否已经开始监听且当前主机与目标主机之间没有NAT端口映射\n" % (ip, port))
            exit()
        return

    def send_bytes(self, input_bytes):
        try:
            self.sk.sendall(input_bytes)
        except:
            print("[ERROR] \tBytes send fail!")

    def send_str(self, input_str):
        try:
            self.sk.sendall(bytes(input_str, encoding="utf-8"))
        except:
            print("[ERROR] \tBytes send fail!")

    def send_file(self, f_name):
        try:
            with open(f_name+".encrypt", "rb") as f:
                content = f.read()
            self.sk.sendall(content)
        except:
            print("[ERROR] \tBytes send fail!")

    def test(self):
        print(self.sk)




def test_listen():
    print("test_listen begin")
    sk = MySocket()
    sk.listen()
    time.sleep(1)
    print(sk.recv())

def test_conn():
    print("test_conn begin")
    sk = MySocket()
    sk.test()
    sk.connect()
    sk.send_str("123456")

if __name__ == "__main__":
    t1 = threading.Thread(target=test_listen)
    t2 = threading.Thread(target=test_conn)

    t1.start()
    time.sleep(1)
    t2.start()












