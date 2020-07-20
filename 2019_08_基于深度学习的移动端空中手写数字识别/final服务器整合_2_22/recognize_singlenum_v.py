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
#modelpath='veritical_CNN_BLSTM_model_800_scale.h5'  #CNN-BLSTM的
#modelpath='oldalldata_CNN_BLSTM_model_1500_scale.h5'
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
def recognize_number(src_addr,model):
    #print('src_addr is %s'%src_addr)
    pre_lost=2#前面去掉15
    post_lost=2#后面去掉15
    windowsize=10#窗长
    hold_position=0
    #clf=SVC(gamma='auto',probability=True)
    #clf.fit(X_train,y_train)
    #joblib.dump(clf, 'cut.model')
    #clf=joblib.load('cut.model')#训练好了的SVM切割模型
    #src_addr='./bigdata/test/'+'036.csv'
    df1=pd.read_csv(src_addr)
    #df=df1
    df=df1[pre_lost:-post_lost].reset_index(drop=True)

    #if(hold_position==0):#如果和数据库中一样
    #    ax=df.ACCx.rolling(window=int(windowsize/2)).mean().values
    #    az=df.ACCz.rolling(window=int(windowsize/2)).mean().values
    #    ay=df.ACCy.rolling(window=int(windowsize/2)).mean().values
    #else:#水平横着拿手机的,z=-x,x=y
    #    az=df.ACCx.rolling(window=int(windowsize/2)).mean().values
    #    ay=df.ACCz.rolling(window=int(windowsize/2)).mean().values
    #    ax=df.ACCy.rolling(window=int(windowsize/2)).mean().values
    #    az=-az
            
    #segment=df

    ifpredict=1
    #print('len segment is %d'%len(segment))
    if(ifpredict==1):
        maxlen=800#每个数字至多这么多时间
        X_test=np.zeros((1,int(maxlen),3))
        try:
            temp=np.zeros((3,maxlen))
            temp[0][0:len(df.ACCx.values)]=df.ACCx.values
            temp[1][0:len(df.ACCx.values)]=df.ACCy.values
            temp[2][0:len(df.ACCx.values)]=df.ACCz.values
            temp=temp.T
            X_test[0]=temp
            X_test[0][0:len(df.ACCx.values)]=StandardScaler().fit_transform(X_test[0][0:len(df.ACCx)])#标准化!
            X_test[0][len(df.ACCx.values):]=0
            
            num_prob=model.predict(X_test)
            num_pred=np.argmax(num_prob)
        except:
            num_pred=0
        return(num_pred)

        #except:
        #    return('0'*digit_total)

#num_pred=recognize_number('./csv/1562831289830.csv',10,clf,model)           
#num_pred=recognize_number('./csv/1565674722469.csv',1,clf,model)   
