# 环境准备（蔡贸俊）

## 工具链与算法库版本

```text
$ gcc --version | head -1
gcc (GCC) 12.3.1 (openEuler 12.3.1-38.oe2403)
$ cmake --version | head -1
cmake version 3.27.9
$ openssl version
OpenSSL 3.0.12 24 Oct 2023 (Library: OpenSSL 3.0.12 24 Oct 2023)
$ gmssl version
GmSSL 3.3.0-dev.1183
$ cat /etc/ld.so.conf.d/gmssl.conf
/usr/local/lib
```

## 准备过程
1. WSL2 安装 openEuler 24.03 LTS（作业推荐系统）。
2. `dnf install -y gcc-c++ cmake openssl-devel`（OpenSSL 开发头文件与编译工具）。
3. 源码编译 GmSSL 3.3.0：`git clone https://github.com/guanzhi/GmSSL.git` → `cmake -B build && cmake --build build -j` → `cmake --install build`，构建全部目标成功（100% Built target ...）。
4. 配置 `echo /usr/local/lib > /etc/ld.so.conf.d/gmssl.conf && ldconfig`，解决 libgmssl 链接与运行时的库路径问题（详见问题与反思第1条）。