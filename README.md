# 说明

Linux 或者win运行

改：

### 1.building

Makefile66-69行注释部分，具体看你们如何挂载a.img

### 2. keymap

bochsrc45-49行，自行查询设置

### 3. ROM images

Bochsrc18-19行，更改为自己的地址

其他编译、运行和书上相同









# 功能

### 已实现

相对路径/绝对路径 访问

文件/文件夹 增删改



### 命令

+ cd
+ mkdir
+ create
+ write
+ read
+ rm
+ ls



## 未实现

### 控制台

- 编辑器？？
- 打开
- 关闭
- 修改
- 删除





## 修改

inlclude/sys/global.h:改了文件系统通信message的结构，区分文件/文件夹

/* macros for messages */

\#define FD      u.m3.m3i1

\#define PATHNAME    u.m3.m3p1

\#define FLAGS       u.m3.m3i1

\#define NAME_LEN    u.m3.m3i2

\#define CNT         u.m3.m3i2

\#define REQUEST     u.m3.m3i2

\#define PROC_NR     u.m3.m3i3

\#define DEVICE      u.m3.m3i4

\#define MODE u.m3.m3i4//添加

\#define POSITION    u.m3.m3l1

\#define BUF         u.m3.m3p2

\#define OFFSET      u.m3.m3i2

\#define WHENCE      u.m3.m3i3

 

\#define PID         u.m3.m3i2

/* #define  STATUS      u.m3.m3i1 */

\#define RETVAL      u.m3.m3i1





fs文件夹  
link.c  把处理通讯和unlink拆开，实现文件夹删除

main.c   currentDir_inode 初始化

misc.c 实现绝对路径/相对路径访问

open.c 和link差不多，多了个openDir



fs.h 超级块



实现：文件夹（切换currentDir_inode、文件夹是特殊文件）

文件夹是一个存储 dir_entry（定义在fs.h里）目录的文件



主要困难

指针

对文件夹的理解

inode->i_cnt 的处理（计数，有多少个进程在使用）

+ 如果处理不好就会无法删除
+ 所以要在适当的时候put_inode

连续访问的逻辑







