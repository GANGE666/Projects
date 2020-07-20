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
#clf=joblib.load('cut.model')#训练好了的SVM切割模型
#modelpath='CNN_BLSTM_model_800_scale.h5'  #CNN-BLSTM的
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

#是文件路径,digit_total是数字总数,clf是SVM，model是数字识别模型
def recognize_number(src_addr,digit_total,clf,model):
    #print('src_addr is %s'%src_addr)
    pre_lost=2#前面去掉2
    post_lost=2#后面去掉2
    windowsize=10#窗长
    hold_position=0
    #clf=SVC(gamma='auto',probability=True)
    #clf.fit(X_train,y_train)
    #joblib.dump(clf, 'cut.model')
    #clf=joblib.load('cut.model')#训练好了的SVM切割模型
    #src_addr='./bigdata/test/'+'036.csv'
    df1=pd.read_csv(src_addr)
    df=df1[pre_lost:-post_lost].reset_index(drop=True)

    if(hold_position==0):#如果和数据库中一样
        ax=df.ACCx.rolling(window=int(windowsize/2)).mean().values
        az=df.ACCz.rolling(window=int(windowsize/2)).mean().values
        ay=df.ACCy.rolling(window=int(windowsize/2)).mean().values
    else:#水平横着拿手机的,z=-x,x=y
        az=df.ACCx.rolling(window=int(windowsize/2)).mean().values
        ay=df.ACCz.rolling(window=int(windowsize/2)).mean().values
        ax=df.ACCy.rolling(window=int(windowsize/2)).mean().values
        az=-az
            
    ax[0:int(windowsize/2)-1]=ax[int(windowsize/2)-1:int(windowsize/2)*2-2]
    ay[0:int(windowsize/2)-1]=ay[int(windowsize/2)-1:int(windowsize/2)*2-2]
    az[0:int(windowsize/2)-1]=az[int(windowsize/2)-1:int(windowsize/2)*2-2]
    ax=ax-ax.mean()
    ay=ay-ay.mean()
    az=az-az.mean()
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

    y_pred=np.zeros((len(df)))
    index=0
    while index+windowsize<df.shape[0]:
        seg=np.arange(index,index+windowsize)
        ax_seg=ax[seg]
        ay_seg=ay[seg]
        az_seg=az[seg]
        dx_seg=dx[seg]
        dy_seg=dy[seg]
        dz_seg=dz[seg]
        energy_seg=ax_seg*ax_seg+az_seg*az_seg
        avg_acc=np.sum(ax_seg+az_seg)/len(seg)
        avg_var=np.sum(dx_seg+dz_seg)/len(seg)
        avg_energy=np.sum(energy_seg)/len(seg)
        delta_ax_seg=delta_ax[seg]
        delta_ay_seg=delta_ay[seg]
        delta_az_seg=delta_az[seg]
        avg_delta=np.sum(abs(delta_ax_seg)+abs(delta_ay_seg)+abs(delta_az_seg))/len(seg)
        temp=np.array([avg_delta,avg_acc,avg_var,avg_energy])
        temp=temp.reshape(-1,len(temp))
        yprob=clf.predict_proba(temp)
        y_pred[index:index+windowsize]=np.argmax(yprob)
        index=index+windowsize

    #震荡区，一会儿为1一会儿为0的，就说明是0，如何定位？？？很接近了，判断再仔细一些
    current_area_index_begin=0#当前块开始
    current_area_len=0#当前块长度
    temp_index_ptr=0
    current_flag=y_pred[0]
    index=0#索引

    quiet_seg_begin=[]#开始位置
    quiet_seg_len=[]#长度
    #换个思路，够宽的静音区就不动，其它的为1,找出10个/或者除了最后一段的最长的10个静音区
    #如果在最后，则取前11个，去掉最靠近结尾的那个,不是静音区的全填1
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

        if(current_flag==0):#如果是静音区
            quiet_seg_begin.append(current_area_index_begin)
            quiet_seg_len.append(current_area_len)

        index=index+current_area_len

    if(len(quiet_seg_len)<digit_total):
        return('0'*digit_total)
        
    quiet_seg_len=np.array(quiet_seg_len)
    
    max_10_item=quiet_seg_len.argsort()[-digit_total:][::-1]

    quiet_seg=[]
    find_flag=0
    for i in range(digit_total):
        if((quiet_seg_begin[max_10_item[i]]+quiet_seg_len[max_10_item[i]]+10*windowsize)<len(y_pred)):
            quiet_seg.append(np.arange(quiet_seg_begin[max_10_item[i]],quiet_seg_begin[max_10_item[i]]+quiet_seg_len[max_10_item[i]]))
        elif(i==digit_total-1 and (quiet_seg_begin[max_10_item[i]]+quiet_seg_len[max_10_item[i]])<len(y_pred)):#如果是最后一个区间被划很长
            quiet_seg.append(np.arange(quiet_seg_begin[max_10_item[i]],quiet_seg_begin[max_10_item[i]]+quiet_seg_len[max_10_item[i]]))
        else:
            find_flag=1


    if(find_flag==1):
        find_flag=0
        try:
            max_10_item=quiet_seg_len.argsort()[-(digit_total+1):][::-1]
            if(quiet_seg_begin[max_10_item[digit_total]]+quiet_seg_len[max_10_item[digit_total]])<len(y_pred):
                quiet_seg.append(np.arange(quiet_seg_begin[max_10_item[digit_total]],quiet_seg_begin[max_10_item[digit_total]]+quiet_seg_len[max_10_item[digit_total]]))
            else:
                max_10_item=quiet_seg_len.argsort()[-(digit_total+2):][::-1]
                if(quiet_seg_begin[max_10_item[digit_total+1]]+quiet_seg_len[max_10_item[digit_total+1]])<len(y_pred):
                    quiet_seg.append(np.arange(quiet_seg_begin[max_10_item[digit_total+1]],quiet_seg_begin[max_10_item[11]]+quiet_seg_len[max_10_item[digit_total+1]]))
                else:
                    print(src_addr)
        except:
            return('0'*digit_total)
    quiet_seg=np.array(quiet_seg)


    y_pred_one=np.ones((len(y_pred)))

    for i in range(len(quiet_seg)):
        y_pred_one[quiet_seg[i]]=0

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
            if(current_flag==y_pred_one[temp_index_ptr]):#如果后面还有
                current_area_len=current_area_len+windowsize
                while(temp_index_ptr+windowsize<len(y_pred)):
                    temp_index_ptr=temp_index_ptr+windowsize
                    if(current_flag==y_pred_one[temp_index_ptr]):#如果是连续的
                        current_area_len=current_area_len+windowsize
                    else:#否则到此结束
                        break
        if(current_flag==1):
            segment.append(df[current_area_index_begin:current_area_index_begin+current_area_len])
        index=index+current_area_len

    ifpredict=1
    #print('len segment is %d'%len(segment))
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
        #print('len segment is %d'%len(segment))
        if(1):
            for i in range(digit_total):
                temp=np.zeros((3,maxlen))
                temp[0][0:len(segment[i].ACCx)]=segment[i].ACCx
                temp[1][0:len(segment[i].ACCy)]=segment[i].ACCy
                temp[2][0:len(segment[i].ACCz)]=segment[i].ACCz
                temp=temp.T
                X_test[i]=temp
            for k in range(len(X_test)):
                X_test[k][0:len(segment[k].ACCx)]=StandardScaler().fit_transform(X_test[k][0:len(segment[k].ACCx)])#标准化!
                X_test[k][len(segment[k].ACCx):]=0
            num_prob=model.predict(X_test)
            print('predict %d'%i)
            num_pred=np.zeros(len(num_prob),dtype=np.int32)
            for i in range(len(num_prob)):
                num_pred[i]=np.argmax(num_prob[i])
            print(num_pred)
            return(num_pred)


        #except:
        #    return('0'*digit_total)

#num_pred=recognize_number('./csv/1562831289830.csv',10,clf,model)           
#num_pred=recognize_number('./csv/1562832894265.csv',4,clf,model)   
