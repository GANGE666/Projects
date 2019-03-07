# 基于androguard通过识别annotations快速导出app的JavascriptInterface

### 基本介绍
此脚本是在2019年一月第二届xman冬令营期间，为方便分析app中的JavascriptInterface漏洞并尝试在webview中利用时，编写的。

***
### 完成情况
代码比较简单，将app中的JavascriptInterface相关信息导出至文件中。

以快手app为例：

- KuaiShou.apk-method-list.txt

可以通过函数名，迅速判断出可能存在安全隐患及隐私泄露的函数。

![2019-03-07-18-21-46.png](https://github.com/GANGE666/Projects/raw/master/other/2019_01_JavascriptInterface/2019-03-07-18-21-46.png)

- KuaiShou.apk-JavascriptInterface-list.txt

显示函数所在的具体位置，方便在jeb中定位相应代码。

![2019-03-07-18-23-39.png](https://github.com/GANGE666/Projects/raw/master/other/2019_01_JavascriptInterface/2019-03-07-18-23-39.png)

- KuaiShou.apk-JavascriptInterface.java

将所有函数的java源码打印出来，方便研究者对这些函数有个大致了解，省去在jeb中定位函数的时间。

![2019-03-07-18-26-11.png](https://github.com/GANGE666/Projects/raw/master/other/2019_01_JavascriptInterface/2019-03-07-18-26-11.png)


***
### 相关文件
- [源代码](./JavascriptInterface.py)
- [KuaiShou.apk-method-list.txt](./KuaiShou.apk-method-list.txt)
- [KuaiShou.apk-JavascriptInterface-list.txt](./KuaiShou.apk-JavascriptInterface-list.txt)
- [KuaiShou.apk-JavascriptInterface.java](./KuaiShou.apk-JavascriptInterface.java)
- [KuaiShou.apk](./KuaiShou.apk)
