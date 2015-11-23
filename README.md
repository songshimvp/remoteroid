# remoteroid
Automatically exported from code.google.com/p/remoteroid

Remoteroid is a remote control solution for Android platform, including handy file transfer and notify-to-PC.

You can control your Phone on your PC without leaving your hands from keyboard and mouse.

Remoteroid now supports Windows-based PC only. 

Your phone should able to get root access to use Remoteroid.

## Server端编译错误解决：（重新装了VS2010）
http://blog.csdn.net/songshimvp1/article/details/49999989     ——error LNK2038、error LNK1123

## Client端错误解决：
找到.classpath文件，对应修改为：classpathentry kind="src" path="src"  ；<BR>
同时加载ActionbarSherlockLib和android两个文件夹，进行编译；
