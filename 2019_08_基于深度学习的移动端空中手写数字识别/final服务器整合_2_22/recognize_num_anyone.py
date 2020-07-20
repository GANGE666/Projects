import csv
import numpy as np
import pandas as pd
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
np.random.seed(seed=42)

#拿的姿势，从手机端获得
hold_position=0

#由应用决定，是4/6
#digit_total=10
#clf=joblib.load('oldcut_2.model')#训练好了的SVM切割模型
#modelpath='veritical_CNN_BLSTM_model_800_scale_best.h5'  #CNN-BLSTM的
#model=load_model(modelpath)#数字识别模型
def plot_confusion_matrix(y_true, y_pred, classes,
                          normalize=False,
                          title=None,
                          cmap=plt.cm.Blues):
    if not title:
        title = 'Confusion matrix'

    # Compute confusion matrix
    cm = confusion_matrix(y_true, y_pred)
    # Only use the labels that appear in the data
    classes = classes[unique_labels(y_true, y_pred)]

    fig, ax = plt.subplots()
    im = ax.imshow(cm, interpolation='nearest', cmap=cmap)
    ax.figure.colorbar(im, ax=ax)
    # We want to show all ticks...
    ax.set(xticks=np.arange(cm.shape[1]),
           yticks=np.arange(cm.shape[0]),
           # ... and label them with the respective list entries
           xticklabels=classes, yticklabels=classes,
           title=title,
           ylabel='True label',
           xlabel='Predicted label')

    # Rotate the tick labels and set their alignment.
    plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
             rotation_mode="anchor")

    # Loop over data dimensions and create text annotations.
    fmt = '.2f' if normalize else 'd'
    thresh = cm.max() / 2.
    for i in range(cm.shape[0]):
        for j in range(cm.shape[1]):
            ax.text(j, i, format(cm[i, j], fmt),
                    ha="center", va="center",
                    color="white" if cm[i, j] > thresh else "black")
    fig.tight_layout()
    plt.show()
    return ax


        
def recognize_number(src_addr,model):
    #print('src_addr is %s'%src_addr)
    pre_lost=2#前面去掉2
    post_lost=2#后面去掉2
    windowsize=10#窗长
    
    df1=pd.read_csv(src_addr)
    df=df1[pre_lost:-post_lost].reset_index(drop=True)

    ax=df.ACCx.values
    ay=df.ACCy.values
    az=df.ACCz.values
            
    energy_x=ax*ax
    energy_y=ay*ay
    energy_z=az*az
    energy_total=energy_x+energy_y+energy_z

    dx=(df.ACCx.rolling(window=int(windowsize/2)).var()).values
    dy=(df.ACCy.rolling(window=int(windowsize/2)).var()).values
    dz=(df.ACCz.rolling(window=int(windowsize/2)).var()).values
    #对于开头的NaN的处理
    dx[0:int(windowsize/2)-1]=dx[int(windowsize/2)-1:int(windowsize/2)*2-2]
    dy[0:int(windowsize/2)-1]=dy[int(windowsize/2)-1:int(windowsize/2)*2-2]
    dz[0:int(windowsize/2)-1]=dz[int(windowsize/2)-1:int(windowsize/2)*2-2]

    #加速度的变化率
    delta_ax=np.zeros((len(ax)))
    delta_ay=np.zeros((len(ax)))
    delta_az=np.zeros((len(ax)))
    for i in range(len(ax)-1):
        delta_ax[i]=ax[i+1]-ax[i]
        delta_ay[i]=ay[i+1]-ay[i]
        delta_az[i]=az[i+1]-az[i]

    quiet_flag=1
    write_seg_test=[]
    quiet_seg_test=[]
    state_lable_test=np.zeros((df.shape[0]),dtype=np.int32)#正确的是什么样的
    last_index=0
    for row in range(df.shape[0]):
        if str(df['Keydown'][row])!=str(-1):#如果发生了一次按键
            if(quiet_flag==1):
                write_seg_test.append([last_index,row])
            else:
                quiet_seg_test.append([last_index,row])
            state_lable_test[last_index:row]=quiet_flag
            quiet_flag=quiet_flag^1
            last_index=row
            
    index=0#总的索引

    num_seg=[]

    may_num_flag=0#可能是数字段
    num_seg_begin=0#数字段内开始位置
    num_seg_end=0#数字段内结束位置
    num_write_length_add=0#数字段长度计数
    num_gap_length_add=0#数字段掉下去间隔计数

    may_quiet_flag=0#可能是静止段标志
    quiet_seg_begin=0#静止段内开始位置
    quiet_seg_end=0#静止段结束位置
    quiet_seg_length=0

    y_pred=np.zeros(len(ax))
    seg_num=0#总共发现了多少个段落

    current_flag=0#当前是在搜索什么，判断这个段目前是什么

    #长度和阈值达到才是在写
    heigth_OK=0#阈值达到
    length_OK=0#长度达到

    write_thre_1=2.5#能量第1阈值
    write_thre_2=3#能量第2阈值
    quiet_thre=2#静止区域
    num_min_len=30#书写区域长度阈值
    num_max_gap=3#最长间隔
    num_max_wait=15
    counter=1
    while(index<len(ax)):#没搜寻结束
        #对当前属性进行判断
        if(energy_total[index]>=write_thre_2):#阈值达到了要求，可能是数字段
            heigth_OK=1#阈值达到
            if(may_quiet_flag==1 and may_num_flag==1):
                num_write_length_add=num_write_length_add+quiet_seg_length
                may_quiet_flag=0
            elif(may_num_flag==0):
                may_num_flag=1
                num_seg_begin=index
                num_write_length_add=1
            elif(may_quiet_flag==0 and may_num_flag==1):#之前没有静止段
                num_write_length_add=num_write_length_add+1#数字段长度+1
            quiet_seg_length=0
            may_quiet_flag=0#取消可能静止段标志
                
        elif(energy_total[index]>=write_thre_1):#达到第一阈值，可能是数字段
            if(may_quiet_flag==1 and may_num_flag==1):
                num_write_length_add=num_write_length_add+quiet_seg_length
            elif(may_num_flag==0):
                may_num_flag=1
                num_seg_begin=index
                num_write_length_add=1
            elif(may_quiet_flag==0 and may_num_flag==1):#之前没有静止段
                num_write_length_add=num_write_length_add+1#数字段长度+1
            quiet_seg_length=0
            may_quiet_flag=0#取消可能静止段标志

        elif(energy_total[index]<=quiet_thre):
            if(may_quiet_flag==0):#如果之前没有打上可能的静止段标记，打标记
                may_quiet_flag=1
                quiet_seg_begin=index
                quiet_seg_length=1
            else:
                quiet_seg_length=quiet_seg_length+1

            if(may_num_flag==1 and quiet_seg_length>=num_max_gap):#如果掉下去了,判定结束
                
                if(num_write_length_add>=num_min_len):#如果是在写，有效
                    num_seg_end=index-quiet_seg_length
                    num_seg.append([num_seg_begin,num_seg_end])
                may_num_flag=0
                num_write_length_add=1       
                #print(index,num_seg_begin,num_seg_end,quiet_seg_length)
                quiet_seg_length=0#判定结束
        else:
            if(may_quiet_flag==0 and may_num_flag==1):#之前没有静止段
                num_write_length_add=num_write_length_add+1#数字段长度+1
                
        index=index+1


        
    if(may_num_flag==1):
        if(num_write_length_add>=num_min_len):#如果是在写，有效
            num_seg_end=index-quiet_seg_length
            if(num_seg_end>=len(ax)):
                num_seg_end=len(ax)-1
            num_seg.append([num_seg_begin,num_seg_end])
            may_num_flag=0

        
    for item in num_seg:
        y_pred[item[0]:item[1]]=1   

    index=0
    segment=[]#有效的段,应该是digit_total个
    while index+windowsize<len(y_pred):
        current_area_index_begin=index
        current_flag=y_pred[index]

        #向后查找,确定当前块有多长
        current_area_len=0
        if(index+windowsize<len(y_pred)):
            temp_index_ptr=index+windowsize
            current_area_len=windowsize
            if(current_flag==y_pred[temp_index_ptr]):#如果后面还有
                current_area_len=current_area_len+windowsize
                while(temp_index_ptr+windowsize<len(y_pred)):
                    temp_index_ptr=temp_index_ptr+windowsize
                    if(current_flag==y_pred[temp_index_ptr]):#如果是连续的
                        current_area_len=current_area_len+windowsize
                    else:#否则到此结束
                        break
        if(current_flag==1):
            segment.append(df[current_area_index_begin:current_area_index_begin+current_area_len])
        index=index+current_area_len
    digit_total=len(segment)
    ifpredict=1
    if(len(segment)<1):
        print('segment is too short')
        return('0')
    #else:
        #print(len(segment),' numbers')
    
    if(ifpredict==1):
        maxlen=800#每个数字至多这么多时间
        X_test=np.zeros((digit_total,int(maxlen),3))
        if(len(segment)>digit_total):
            s_len=np.zeros(len(segment),dtype=np.int32)
            for i in range(len(segment)):
                s_len[i]=len(segment[i])
            max_item=s_len.argsort()[-(digit_total):][::-1]
            rm_item=list(set(range(len(segment)))-set(max_item))
            for k in rm_item:
                del segment[k]
        if(1):
            try:
                for i in range(digit_total):
                    temp=np.zeros((3,maxlen))
                    temp[0][0:len(segment[i].ACCx)]=segment[i].ACCx
                    temp[1][0:len(segment[i].ACCy)]=segment[i].ACCy
                    temp[2][0:len(segment[i].ACCz)]=segment[i].ACCz
                    temp=temp.T
                    X_test[i]=temp
                for k in range(len(X_test)):
                    X_test[k]=StandardScaler().fit_transform(X_test[k])#标准化!
                num_prob=model.predict(X_test)

                num_pred=np.zeros(len(num_prob),dtype=np.int32)
                for i in range(len(num_prob)):
                    num_pred[i]=np.argmax(num_prob[i])
            except:
                num_pred=0
            print(num_pred)
            #plt.subplot(211)
            #plt.plot(range(0,len(energy_total)),energy_total,color='r',label='sum_energy')
            #plt.title('sum statistics ',fontsize=20)
            #plt.legend(fontsize=18)

            #plt.subplot(212)
            #plt.title(u'数字分割',fontsize=20)
            #plt.plot(np.arange(0,len(state_lable_test)),state_lable_test,color='r',label='true')
            #plt.plot(np.arange(0,len(y_pred)),y_pred,color='b',label='predict')
            #plt.legend(fontsize=18)
            #plt.show()
            return(num_pred)
#num_pred=recognize_number('./csv/1562831289830.csv',10,clf,model)           
#num_pred=recognize_number('./csv/28013.csv',10,clf,model)   
