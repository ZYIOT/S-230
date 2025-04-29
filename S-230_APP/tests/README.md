#单元测试程序

### 环境
* msys64  https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20200720.exe
* gcc 编译环境
* make
* cmake

##### gcc 安装
```
#搜索 pacman -Ss gcc
pacman -S mingw64/mingw-w64-x86_64-gcc --disable-download-timeout
pacman -S mingw64/mingw-w64-x86_64-make
pacman -S mingw64/mingw-w64-x86_64-toolchain
pacman -S mingw64/mingw-w64-x86_64-cmake
```

### 编译
```
mkdir build
cd build
cmake ..
mingw32-make
```

### msys64 安装问题
##### 错误：mingw32: 密钥 "4A6129F4E4B84AE46ED7F635628F528CF3053E04" 未知 错误：无法远程查找到密钥 "4A6129F4E4B84AE46ED7F635628F528CF3053E04"
```
curl -O http://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz
curl -O http://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz.sig
pacman-key --verify msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz.sig

pacman -U msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz

# 如果上一句出错
pacman -U --config <(echo) msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz

# 如果还是出错
rm -r /etc/pacman.d/gnupg/
pacman-key --init
pacman-key --populate msys2

# 最后
pacman -Syu
```

#####  repo.msys2.org : Operation too slow. Less than 1 bytes/sec transferred the last 10 seconds 获取文件
添加`--disable-download-timeout`参数
```
pacman -Syu --disable-download-timeout
```

或添加清华的源
```
编辑 /etc/pacman.d/mirrorlist.mingw32 ，在文件开头添加：
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686

编辑 /etc/pacman.d/mirrorlist.mingw64 ，在文件开头添加：
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64

编辑 /etc/pacman.d/mirrorlist.msys ，在文件开头添加：
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch

然后执行 pacman -Sy 刷新软件包数据即可。
```
