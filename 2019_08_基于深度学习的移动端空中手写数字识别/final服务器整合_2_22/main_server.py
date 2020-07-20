import socket
import time
import threading
import excel2csv
import requests
import binascii
from twilio.rest import Client
import recognize_num_svm
import recognize_num_anyone
import recognize_num_v
import recognize_singlenum_v
import csv
import numpy as np
import pandas as pd
from matplotlib.image import imread
from matplotlib import pyplot as plt
import os
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.externals import joblib
import math
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.utils.multiclass import unique_labels
from sklearn.metrics import confusion_matrix
from sklearn.preprocessing import StandardScaler
from keras.models import Model, load_model
from keras.layers import Dense, Input, LSTM, Embedding, Dropout, Activation,Bidirectional, GlobalMaxPool1D,Flatten
from keras.layers import RepeatVector
from keras.callbacks import ModelCheckpoint
from keras import regularizers
from keras.models import Sequential
from keras.layers.merge import concatenate
from keras.layers.convolutional import Conv1D,MaxPooling1D
from keras.layers import Flatten
import keras
import warnings
warnings.filterwarnings('ignore')

import many_genarate_number_seq
import send_message_mode



SOCKET_PORT = 12345

Recording = True
keydown_time_list = []

clf=joblib.load('oldcut_2.model')#训练好了的SVM切割模型

modelpath_v='vertical_free_CNN_BLSTM_model_800_scale_8_20.h5'
model_v=load_model(modelpath_v)#vertical数字识别模型

modelpath_h='horizon_free_CNN_BLSTM_model_800_scale_8_23.h5'
model_h=load_model(modelpath_h)#vertical数字识别模型


number_num=4#4位验证码


v_or_h=0#0表示v，1表示h


def Getch():
    def _GetchUnix():
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

    def _GetchWindows():
        import msvcrt
        return msvcrt.getch()

    try:
        impl = _GetchWindows()
    except ImportError:
        impl = _GetchUnix()
    return impl

def Record_keyboard():
    # 记录按键时间的线程
    while Recording:
        ch = ord(Getch()) & 0xFF
        ch_time = int(time.time()*1000)
        print("KeyDown: %c time: %d" % (ch, ch_time))

        if ch == 27:  # ESC 退出
            exit(0)

        ch = ord('D')

        keydown_time_list.append([ch_time, chr(ch)])

# 生成csv并标记按键时刻
def genarate_csv_and_mark(filename):
    global keydown_time_list
    global Recording
    csvname=excel2csv.do_mark(filename, keydown_time_list)
    keydown_time_list = []
    Recording = True
    return csvname


def to_unicode(string):
    ret = ''
    for v in string:
        ret = ret + hex(ord(v)).upper().replace('0X', '\\u')
    return ret


# 生成识别的数字字符串，在这里进行识别
def genarate_number_seq(filename,current_activity):
    global clf,model_h,model_v,number_num,v_or_h

    df=pd.read_csv(filename)
    gravity_z=df.ADGz.mean()
    gravity_y=df.ADGy.mean()
    if(gravity_z>=5 or gravity_y>=5):#如果是横着写的
        v_or_h=1
        if(current_activity==1):
            number = recognize_num_v.recognize_number(filename,number_num,clf,model_h)
        elif(current_activity==2):
            #print('110 test')
            #number = recognize_num_v.recognize_number(filename,number_num,clf,model_h)
            number=recognize_num_anyone.recognize_number(filename,model_h)
        elif(current_activity==3 or current_activity==4):
            number = recognize_singlenum_v.recognize_number(filename,model_h)
        elif(current_activity==5):
            number=recognize_num_anyone.recognize_number(filename,model_h)
    else:
        v_or_h=0
        if(current_activity==1):
            number = recognize_num_v.recognize_number(filename,number_num,clf,model_v)
        elif(current_activity==2):
            number = recognize_num_anyone.recognize_number(filename,model_v)
            #number = recognize_num_v.recognize_number(filename,number_num,clf,model_v)
        elif(current_activity==3 or current_activity==4):
            number = recognize_singlenum_v.recognize_number(filename,model_v)
        elif(current_activity==5):
            number=recognize_num_anyone.recognize_number(filename,model_v)

    print('current_activity = %d '%current_activity)        
        

    res=''
    if(current_activity == 1 or current_activity == 2 or current_activity == 5):
        for num in number:
            res=res+str(num)
    else:
        res=str(number)
    print("成功识别书写的数字串为：%s" % res)

    return res

# 调用API获取文字的位置
def genarate_location_info(loc_str):
    print(loc_str)
    ak = "**************************"
    url = "http://api.map.baidu.com/reverse_geocoding/v3/?ak="+ak+"&output=json&coordtype=wgs84ll&location=" + loc_str
    # print(url)
    res = requests.get(url)
    # print(res.text)
    return res.json()

# 发送短信 https://www.twilio.com/console
def send_message(res_json):

    print(res_json)

    current_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    current_loc = res_json['result']['addressComponent']['country'] + res_json['result']['formatted_address'] + res_json['result']['business'] + res_json['result']['addressComponent']['street']
    jindu = res_json['result']['location']['lat']
    weidu = res_json['result']['location']['lng']

    message = "Help me in %s (%f,%f)" % (current_loc,jindu, weidu)
    #message = "[Warning] Please help me! I'm in Latitude: %f ,Longitude: %f " % (jindu, weidu)
    # message = "[Warning]"
    print(message)

    account_sid = "******************************"
    auth_token = "******************************"
    client = Client(account_sid, auth_token)
    message_ = client.messages.create(
        to="+86***********",
        from_="+**********",
        #body=to_unicode(message))
        body=message)
    print(message_.sid)
    print(message_.body)

import socketserver
message_sender = send_message_mode.SendMessage()
class Myserver(socketserver.BaseRequestHandler):
    
    timeout = 100
    current_activity=0

    def handle(self):
        global Recording

        print("client connect!")

        conn = self.request

        size = 0

        while True:
            op_code = conn.recv(1)
            if op_code and op_code != bytes(0):
                pass
            else:
                continue

            op_code = int.from_bytes(op_code, byteorder='big')
            # print(op_code)

            # 根据op_code的值来执行相应的动作
            if op_code == 0xF0:

                # 计算延迟
                conn.sendall(bytes(str(int(time.time()*1000)), encoding="utf-8"))
                pass


            elif op_code < 0xDF:
                # 0xD1 Activity1
                # 0xD2 Activity2
                # 0xD3 Activity3
                # 0xD4 Activity4
                if op_code == 0xD1:
                    self.current_activity = 1
                    number_num=4#准备识别验证码
                elif op_code == 0xD2:
                    self.current_activity = 2
                    number_num=3#准备识别110
                    #print('number_num = %d'%number_num)
                elif op_code == 0xD3:
                    self.current_activity = 3
                    number_num=1#单个数字
                elif op_code == 0xD4:
                    self.current_activity = 4
                    number_num=1#单个数字
                elif op_code == 0xD5:
                    self.current_activity = 5

                conn.sendall(bytes(str(int(time.time() * 1000)), encoding="utf-8"))
                pass



            
            elif op_code == 0xF1:

                # 接受数据
                if size > 0:
                    current_size = 0
                    file_name = "./excel/" + str(int(time.time()*1000))+".xls"
                    with open(file_name, 'wb') as f:
                        while True:
                            if current_size == size:
                                break
                            data = conn.recv(1024)
                            f.write(data)
                            current_size += len(data)

                    # conn.sendall((0xE1).to_bytes(1, byteorder='big'))  # 通知客户机传输结束    app端取消了接受0xE1，防止0xE1与后面的数字连起来
                    #print("Save file: %s" % file_name)

                    csv_file_name=genarate_csv_and_mark(file_name)
                    res = genarate_number_seq(csv_file_name,self.current_activity)#csv文件是file_name
                    #print('recognize result is'+res)
                    conn.sendall(bytes(str(res), encoding="utf-8"))     # 将生成的序列返回app
                    nums=''
                    for num in res:
                        nums=nums+str(num)+' '
                    # 参数1:数字个数 参数2:csv文件名
                    if(self.current_activity==1):#activity1服务器端可以显示一次
                        os.system('python showfig.py '+str(len(res))+' '+nums)
                    elif(self.current_activity==3):#activity3服务器端画出单个数字
                        if(v_or_h==0):
                            os.system('python showsingle.py '+csv_file_name +' '+str(res))
                        elif(v_or_h==1):
                            os.system('python showsingle_horizon.py '+csv_file_name +' '+str(res))
                        


                else:  # 错误处理
                    pass

            # 接受位置信息
            elif op_code == 0xF4:
                str_len = conn.recv(4)
                str_len = int(str_len)

                recv_data = conn.recv(str_len)
                message_sender.handle_recv(recv_data)

            elif op_code == 0xF2:

                # 数据传输结束，传输hash
                pass

            elif op_code == 0xF3:

                # 文件大小
                size = int(str(conn.recv(1024), encoding="utf-8"))
                #print("file size %d" % size)
                conn.sendall((0xE3).to_bytes(1, byteorder='big'))  # 允许客户机开始传输

                Recording = False

            elif op_code == 0xFE:

                # 客户端开始记录，重制键盘按键记录线程
                Recording = True

            elif op_code == 0xFF:

                # 客户端关闭，结束通信
                print("client close")
                print(conn.addr)
                self.request.close()
                break

    def handle_timeout(self):
        print("Timeout")
        self.request.close()




if __name__ == "__main__":

    #recording = threading.Thread(target=Record_keyboard)
    #recording.start()

    # My_IP = socket.gethostbyname(socket.getfqdn(socket.gethostname()))
    # print("current ip: " + My_IP)
    #sushe_ip = "10.133.167.197"
    sushe_ip="192.168.137.129"

    num_pred=recognize_num_v.recognize_number('./csv/28013.csv',10,clf,model_v) 
    num_pred=recognize_num_v.recognize_number('./csv/28013.csv',10,clf,model_h)     
    server = socketserver.ThreadingTCPServer((sushe_ip, SOCKET_PORT), Myserver)
    server.serve_forever()


