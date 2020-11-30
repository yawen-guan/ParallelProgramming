## homework 3

#### 题目

> 利用LLVM (C、C++)或者Soot (Java)等工具检测多线程程序中潜在的数
> 据竞争以及是否存在不可重入函数,给出案例程序并提交分析报告。

#### 项目结构

```
.
├── checkcode
│   ├── pom.xml
│   ├── src
│   │   └── main
│   │       └── java
│   │           └── com
│   │               └── Gw
│   │                   └── CheckService
│   │                       ├── CheckApp.java
│   │                       └── Check.java
│   └── target
│       └── CheckService-1.0-SNAPSHOT.jar
├── code
│   ├── multithread
│   ├── multithread.bc
│   ├── multithread.c
│   ├── multithread.ll
│   ├── result.txt
│   └── run.sh
├── README.md
└── run.sh
```



#### 编译运行

运行项目根目录下的脚本run.sh即可。



#### 项目内容

1）分析code文件夹下的multithread.c程序中的数据竞争关系（利用ThreadSanitizer）

​	  项目根目录下的run.sh会调用code文件夹下的run.sh，后者的内容为：

```bash
clang -fsanitize=thread -g -O1 multithread.c -o multithread

./multithread >result.txt  2>&1


clang multithread.c -emit-llvm -c -o multithread.bc

clang multithread.c -emit-llvm -S -c -o multithread.ll
```

则ThreadSanitizer分析的结果输出到code文件夹下的result.txt中。



2）分析code文件夹下的multithread.c程序中每一个函数是否可重用

​    （利用codecheck文件夹下用Java写的小应用）



#### 输出结果

result.txt

```bash
==================
WARNING: ThreadSanitizer: data race (pid=28516)
  Write of size 4 at 0x000000f122f8 by thread T2:
    #0 Thread2 /media/gwen/DATA/A_GW/Courses/Parallel_and_distributed_computing/hw3/code/multithread.c:17:11 (multithread+0x4b252d)

  Previous write of size 4 at 0x000000f122f8 by thread T1:
    #0 Thread1 /media/gwen/DATA/A_GW/Courses/Parallel_and_distributed_computing/hw3/code/multithread.c:12:11 (multithread+0x4b24fd)

  Location is global 'Global' of size 4 at 0x000000f122f8 (multithread+0x000000f122f8)

  Thread T2 (tid=28519, running) created by main thread at:
    #0 pthread_create <null> (multithread+0x423a8b)
    #1 main /media/gwen/DATA/A_GW/Courses/Parallel_and_distributed_computing/hw3/code/multithread.c:70:5 (multithread+0x4b26aa)

  Thread T1 (tid=28518, finished) created by main thread at:
    #0 pthread_create <null> (multithread+0x423a8b)
    #1 main /media/gwen/DATA/A_GW/Courses/Parallel_and_distributed_computing/hw3/code/multithread.c:69:5 (multithread+0x4b2694)

SUMMARY: ThreadSanitizer: data race /media/gwen/DATA/A_GW/Courses/Parallel_and_distributed_computing/hw3/code/multithread.c:17:11 in Thread2
==================
ThreadSanitizer: reported 1 warnings
```

codecheck输出结果：

```txt
Thread1 isn't reentrant
Thread2 isn't reentrant
FuncUsingGlobalConst is reentrant
FuncUsingStaticConst is reentrant
FuncUsingGlobalVariable isn't reentrant
FuncUsingStaticVariable isn't reentrant
FuncUsingNewSpace isn't reentrant
FuncCallNonReentrantFunc isn't reentrant
FuncCallReentrantFunc is reentrant
FuncReturnGlobalConst isn't reentrant
FuncReturnStaticConst isn't reentrant
main isn't reentrant

3 functions are reentrant, 9 functions are not
```

