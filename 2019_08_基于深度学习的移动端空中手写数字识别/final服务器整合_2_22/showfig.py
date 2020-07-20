import csv
import numpy as np
import pandas as pd
from matplotlib.image import imread
from matplotlib import pyplot as plt
import sys

if __name__ == "__main__":
    sysnum=int(sys.argv[1])
    print(sysnum)

    numbers=np.zeros(int(sysnum),dtype=np.int32)

    for i in range(len(numbers)):
        numbers[i]=sys.argv[2+i]
        
    

    if(sysnum==4):
        plt.ion()
        n0 = imread('./pic/'+str(numbers[0])+'.png')
        n1 = imread('./pic/'+str(numbers[1])+'.png')
        n2 = imread('./pic/'+str(numbers[2])+'.png')
        n3 = imread('./pic/'+str(numbers[3])+'.png')
        plt.figure()
        plt.subplot(141)
        plt.imshow(n0)  
        plt.axis('off')  
        plt.subplot(142)
        plt.imshow(n1)  
        plt.axis('off') 
        plt.subplot(143)
        plt.imshow(n2)  
        plt.axis('off') 
        plt.subplot(144)
        plt.imshow(n3)  
        plt.axis('off') 
        plt.show()
        plt.pause(2)
        plt.close()
        plt.ioff()
    elif(sysnum==3):
        plt.ion()
        n0 = imread('./pic/'+str(numbers[0])+'.png')
        n1 = imread('./pic/'+str(numbers[1])+'.png')
        n2 = imread('./pic/'+str(numbers[2])+'.png')
        
        plt.figure()
        plt.subplot(131)
        plt.imshow(n0)  
        plt.axis('off')  
        plt.subplot(132)
        plt.imshow(n1)  
        plt.axis('off') 
        plt.subplot(133)
        plt.imshow(n2)  
        plt.axis('off')
        
        plt.show()
        plt.pause(2)
        plt.close()
        plt.ioff()

    
    exit()
