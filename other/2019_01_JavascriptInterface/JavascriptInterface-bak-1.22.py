# -*- coding: utf-8 -*-  

from androguard.misc import AnalyzeAPK
from androguard.core.bytecodes import apk
from androguard.core.bytecodes.dvm import *
from androguard.core.analysis import analysis
from androguard.core.analysis.analysis import Analysis
from androguard.decompiler import decompiler
import sys


if sys.argv[1]:
    apk_path = sys.argv[1]
else:
    # apk_path = "./baidu.apk"
    apk_path = "./app-debug.apk"

print("[*] Begin")

a = apk.APK(apk_path)
f_path = apk_path+"-JavascriptInterface"+".java"
f2_path = apk_path+"-JavascriptInterface-list"+".txt"
f3_path = apk_path+"-method-list"+".txt"

dvm = DalvikVMFormat(a.get_dex())

print("[*] DalvikVMFormat Finish!")

dx = Analysis()
dx.add(dvm)
dvm.set_decompiler(decompiler.DecompilerDAD(dvm, dx))

print("[*] Analysis Finish!")

savedStdout = sys.stdout
with open(f_path, 'w+') as file:
    sys.stdout = file

    f2 = open(f2_path, 'w+')
    f3 = open(f3_path, 'w+')

    for adi in dvm.map_list.get_item_type("TYPE_ANNOTATIONS_DIRECTORY_ITEM"):
        if adi.get_method_annotations() == []:
            continue

        # Each annotations_directory_item contains many method_annotation
        for mi in adi.get_method_annotations():
            # print(repr(dvm.get_method_by_idx(mi.get_method_idx())))

            # Each method_annotation stores an offset to annotation_set_item
            ann_set_item = dvm.CM.get_obj_by_offset(mi.get_annotations_off())

            # a annotation_set_item has an array of annotation_off_item
            for aoffitem in ann_set_item.get_annotation_off_item():

                # The annotation_off_item stores the offset to an annotation_item
                annotation_item = dvm.CM.get_obj_by_offset(aoffitem.get_annotation_off())

                # The annotation_item stores the visibility and a encoded_annotation
                # this encoded_annotation stores the type IDX, and an array of
                # annotation_element
                # these are again name idx and encoded_value's

                encoded_annotation = annotation_item.get_annotation()

                # Print the class type of the annotation

                type_String = dvm.CM.get_type(encoded_annotation.get_type_idx())

                if type_String == "Landroid/webkit/JavascriptInterface;":

                    print("/*")
                    print("@{}".format(type_String))

                    m = dvm.get_method_by_idx(mi.get_method_idx())
                    print(m)

                    f2.write(str(m)+'\n')
                    f3.write(str(m.get_name())+'\n')

                    print("*/")

                    print(m.get_source())

                    print()

    f2.close()
    f3.close()
sys.stdout = savedStdout
print("[*] Output in " + f_path)
print("[*] Finish!")
