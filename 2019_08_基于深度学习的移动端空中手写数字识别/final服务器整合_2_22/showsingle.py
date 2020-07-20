import csv
import numpy as np
import pandas as pd
from matplotlib.image import imread
from matplotlib import pyplot as plt
import sys
import os
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.externals import joblib
import math
from sklearn.decomposition import PCA

def moving_average(a, n=3):
    ret = np.cumsum(a, dtype=float)
    ret[n:] = ret[n:] - ret[:-n]
    return ret[n - 1:] / n

def recognize_number(src_addr,digit_total,numberis):
    print('src_addr is %s'%src_addr)
    pre_lost=5#前面去掉10
    post_lost=5#后面去掉10
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
    
    total_seg=np.zeros((digit_total,3,len(df.ACCx)))
    total_seg[0][0][0:len(df.ACCx)]=df.ACCx.values
    total_seg[0][1][0:len(df.ACCy)]=df.ACCy.values
    total_seg[0][2][0:len(df.ACCz)]=df.ACCz.values
    pca_plot(total_seg,digit_total,numberis)
    
def ownpca(X):  # k is the components you want
    # mean of each feature
    n_samples, n_features = X.shape
    mean = np.array([np.mean(X[:, i]) for i in range(n_features)])
    # normalization
    norm_X = X - mean
    # scatter matrix
    scatter_matrix = np.dot(np.transpose(norm_X), norm_X)
    # Calculate the eigenvectors and eigenvalues
    eig_val, eig_vec = np.linalg.eig(scatter_matrix)

    return eig_val, eig_vec

def pca_plot(number_segs,digit_total,numberis):
    plt.ion()
    plt.figure()
    MAorder = 7
    for curnum in range(digit_total):
        # 移动平均处理,下标为0..MAorder-1这部分数据是NaN，不能进行运算的！很麻烦
        ax = moving_average(number_segs[curnum][0], MAorder)
        az = moving_average(number_segs[curnum][2], MAorder)
        ay = moving_average(number_segs[curnum][1], MAorder)
        print('len ax is'+' '+str(len(ax)))
        # 加速度的积分是速度
        vx = np.zeros(len(ax - MAorder + 1))
        vz = np.zeros(len(az - MAorder + 1))
        vy = np.zeros(len(ay - MAorder + 1))
        vx[0] = ax[MAorder - 1]  # 不然后面j-1越界了，没有初始值
        vz[0] = az[MAorder - 1]
        vy[0] = ay[MAorder - 1]
        enhancex = ax.mean()  # 抵消掉手抖的部分，静止时，测出来x轴自带-0.2左右的加速度，这个值不稳定，用均值代替
        enhancez = az.mean()  # 同理，抵消z轴上的静态加速度
        enhancey = ay.mean()
        for j in range(1, len(ax) - MAorder + 1):
            vx[j] = vx[j - 1] + ax[j + MAorder - 1] - enhancex  # 当前速度=前一刻速度+ (加速度-补偿)
            vz[j] = vz[j - 1] + az[j + MAorder - 1] - enhancez  # 假设20ms内是匀加速
            vy[j] = vy[j - 1] + ay[j + MAorder - 1] - enhancey

        # 位移
        sudu = np.transpose(np.vstack((vx, vy, vz)))

        posy = np.zeros(len(vy))
        posx = np.zeros(len(vx))
        posz = np.zeros(len(vz))

        posx[0] = vx[0]  # 不然后面j-1越界了
        posz[0] = vz[0]
        posy[0] = vy[0]

        for j in range(1, len(vx)):
            posx[j] = posx[j - 1] + vx[j] / 2  # 当前位移=前一刻位移+现在速度，假设在20ms内匀速运动
            posz[j] = posz[j - 1] + vz[j] / 2
            posy[j] = posy[j - 1] + vy[j] / 2

        data = np.transpose(np.vstack((posx, posy, posz)))
        e, d = ownpca(data)
        minindex = 0
        maxindex = 0
        midindex = 0
        for i in range(0, 3):
            #print(e[i])
            if e[i] < e[minindex]:
                minindex = i
            if e[i] > e[maxindex]:
                maxindex = i
        for i in range(0, 3):
            if i != minindex and i != maxindex:
                midindex = i
        #print(d[:, minindex])
        # minv = (d[:, minindex])
        minv = np.abs(d[:, minindex])
        newx = np.cross(minv, [0, 0, 1])
        newy = np.cross(minv, newx)
        # newx = d[:, maxindex]
        # newy = d[:, midindex]
        newz = minv

        newdata = np.linalg.inv(np.transpose(np.vstack((newx, newy, newz))))

        res = np.matmul(data, newdata)

        min0 = min(res[:, 0])
        plt.subplot(1,digit_total,curnum+1)
        
        plt.scatter(-res[:, 1], res[:, 0])
        #plt.scatter(-posz,posx)
        plt.axis('off')
    plt.show()
    plt.title(str(numberis),fontsize=15)
    plt.pause(2)
    plt.close()
    plt.ioff()

        #plt.scatter(-posz,posx)
        #plt.show()

if __name__ == "__main__":
    #clf=joblib.load('cut.model')#训练好了的SVM切割模型
    srcaddr=str(sys.argv[1])
    numberis=str(sys.argv[2])
    recognize_number(srcaddr,1,numberis)
    
