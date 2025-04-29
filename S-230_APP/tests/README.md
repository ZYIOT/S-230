#��Ԫ���Գ���

### ����
* msys64  https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20200720.exe
* gcc ���뻷��
* make
* cmake

##### gcc ��װ
```
#���� pacman -Ss gcc
pacman -S mingw64/mingw-w64-x86_64-gcc --disable-download-timeout
pacman -S mingw64/mingw-w64-x86_64-make
pacman -S mingw64/mingw-w64-x86_64-toolchain
pacman -S mingw64/mingw-w64-x86_64-cmake
```

### ����
```
mkdir build
cd build
cmake ..
mingw32-make
```

### msys64 ��װ����
##### ����mingw32: ��Կ "4A6129F4E4B84AE46ED7F635628F528CF3053E04" δ֪ �����޷�Զ�̲��ҵ���Կ "4A6129F4E4B84AE46ED7F635628F528CF3053E04"
```
curl -O http://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz
curl -O http://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz.sig
pacman-key --verify msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz.sig

pacman -U msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz

# �����һ�����
pacman -U --config <(echo) msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz

# ������ǳ���
rm -r /etc/pacman.d/gnupg/
pacman-key --init
pacman-key --populate msys2

# ���
pacman -Syu
```

#####  repo.msys2.org : Operation too slow. Less than 1 bytes/sec transferred the last 10 seconds ��ȡ�ļ�
���`--disable-download-timeout`����
```
pacman -Syu --disable-download-timeout
```

������廪��Դ
```
�༭ /etc/pacman.d/mirrorlist.mingw32 �����ļ���ͷ��ӣ�
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686

�༭ /etc/pacman.d/mirrorlist.mingw64 �����ļ���ͷ��ӣ�
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64

�༭ /etc/pacman.d/mirrorlist.msys �����ļ���ͷ��ӣ�
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch

Ȼ��ִ�� pacman -Sy ˢ����������ݼ��ɡ�
```
