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



# 功能（预计实现）

### 删除

预计支持当前文件内文件的删除

- unlink测试

  1.删除当前文件夹是不可以的！（报错）

- 实现删除任意地址

  

### 打开

只支持根目录文件或者当前文件夹下

- 实现根据绝对路径打开

  区分打开文件和文件的函数。

  



### 关闭

- 实现返回上一层文件夹（压栈记录？）



### 控制台

- 编辑器？
- 打开
- 关闭
- 修改
- 删除



