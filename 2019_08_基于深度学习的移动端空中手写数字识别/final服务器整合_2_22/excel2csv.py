import pandas as pd
import csv
import os
import numpy as np

def mkdir(path):
    folder = os.path.exists(path) 
    if not folder:                   #判断是否存在文件夹如果不存在则创建为文件夹
        os.makedirs(path)

def save_keydown_list(filename, keydown_time_list):
    t = np.array(keydown_time_list)
    np.save(filename.replace("excel", "list").replace("xls", "txt"), t)

def read_keydown_list_from_file(filename):
    t = np.load(filename.replace("excel", "list").replace("xls", "txt")+".npy")
    return t.tolist()

def do_mark(filename, keydown_time_list):

    df = pd.read_excel(filename)

    time_list_index = 0
    mark_count = 0

    # print(keydown_time_list)

    try:

        for i in range(0, df.shape[0]-1):

            if not keydown_time_list:
                break

            while int(df.loc[i, 'Time']) > int(keydown_time_list[time_list_index][0]):
                time_list_index += 1

            # 上一条记录时间小于按键时间，下一条大于按键时间
            if int(df.loc[i, 'Time']) <= int(keydown_time_list[time_list_index][0]) and int(keydown_time_list[time_list_index][0]) < int(df.loc[i+1, 'Time']):
                df.loc[i, 'Keydown'] = keydown_time_list[time_list_index][1]

                print("[*] Mark %d as %s" % (df.loc[i, 'Time'], str(keydown_time_list[time_list_index][1])))
                time_list_index += 1
                mark_count += 1

                if time_list_index >= len(keydown_time_list):
                    break

    except Exception as e:
        print("Error")
        print(e.with_traceback())

    target_filename = filename.replace("excel", "csv").replace("xls", "csv")
    df.to_csv(target_filename)
    print("Save as %s" % target_filename)
    print("Mark %d points" % mark_count)

    save_keydown_list(filename, keydown_time_list)
    return target_filename





if __name__ == "__main__":
    filename = "./excel/1555349830894.xls"
    key_down_list = read_keydown_list_from_file(filename)
    do_mark(filename, key_down_list)





