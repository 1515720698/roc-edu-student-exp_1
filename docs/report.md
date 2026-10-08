# 《密码系统设计》实验一 实验报告

## 实验名称：嵌入式开发基础

学号姓名：20241328 蔡贸俊

同组成员：20241304 元泓鉴

实验日期：2026-10-08

实验环境：openEuler 24.03 LTS (WSL2)、OpenSSL 3.0.12、GmSSL 3.3.0-dev、gcc 12.3.1

代码仓库：https://gitee.com/xiaoyuanyuan999/roc-edu-student-exp_1

## 一、实验目的

1. 掌握 Linux（openEuler）系统使用与 C 语言开发方法。
2. 掌握 OpenSSL 与 GmSSL 的基本命令用法与开发接口。
3. 掌握 SM2、SM3、SM4 商用密码算法的使用。
4. 通过带签名的数字信封协议理解对称密码与非对称密码算法的组合应用。

## 二、实验环境与准备

实验在 openEuler 24.03 LTS（WSL2）中进行，OpenSSL 使用系统自带 3.0.12，GmSSL 3.3.0 由源码编译安装（cmake 构建全部目标成功），并配置 ldconfig 缓存使其可被正常链接与运行。

![](images/env-setup-caimaojun_1.png)

## 三、实验内容与步骤

### 3.1 任务1：OpenSSL 命令实践

完成 OpenSSL 版本查询、随机数生成、SM4-CBC 对称加解密（加解密往返逐字节一致）；SM3 摘要与 HMAC-SM3 计算并验证雪崩效应；SM2 密钥生成、签名与验签（Verified OK）；speed 性能测试（SM4-CBC 约 110 MB/s，SM3 8KB 块约 320 MB/s）。

![](images/task1-caimaojun_1.png)

![](images/task1-caimaojun_2.png)

![](images/task1-caimaojun_3.png)

![](images/task1-caimaojun_4.png)

### 3.2 任务2：GmSSL 命令实践

使用 GmSSL 命令完成 SM4-CBC 加解密（需显式 PKCS#7 填充）、SM3 摘要与 SM3-HMAC、SM2 密钥生成/签名验签/加密解密，全部验证通过；其中 SM3 摘要与任务1 的 OpenSSL 结果逐字节一致，实现两套实现的交叉验证。

![](images/task2-caimaojun_1.png)

![](images/task2-caimaojun_2.png)

![](images/task2-caimaojun_3.png)

### 3.3 任务3：OpenSSL 命令数字信封 Alice→Bob

Alice（本组，蔡贸俊）生成 16 字节对称密钥 k，SM4 加密明文得 C，用 Bob 公钥 SM2 加密 k 得 KC，用自己的私钥对 C 签名得 S1，发布信封 C||KC||S1 至仓库 exchange/task3/；发送前自验（验签 OK、解密一致）。Bob 拉取后验签、解密钥、解明文，得到"蔡贸俊 20241328"，接收成功。

![](images/task3-envelope_1.png)

![](images/task3-envelope_2.png)

![](images/task3-envelope_3.png)

### 3.4 任务4：GmSSL 命令数字信封 Bob→Alice

Bob（元泓鉴）发送、Alice 接收。接收时发现发送方使用了随机 IV 且未随信封传递，导致全 0 IV 解密首块乱码；利用已知明文（对方学号姓名）由 CBC 性质 IV = D(K,C1) xor P1 恢复出 IV 后解密成功，得"20241304 元泓鉴"。该案例作为协议设计教训写入记录，并已与同组约定后续随信封提交 IV 或固定全 0。

![](images/task4-envelope_1.png)

![](images/task4-envelope_2.png)

![](images/task4-envelope_3.png)

### 3.5 4-6学时 任务1：OpenSSL 库编程

用 OpenSSL EVP API 编程实现 SM2（加密解密、签名验签）、SM3（摘要、HMAC）、SM4（加密解密）调用，往返全部 PASS。实现中明确了 SM2 密钥类型与 EVP 长度参数两个关键点。

![](images/hours46-1-caimaojun_1.png)

![](images/hours46-1-caimaojun_2.png)

### 3.6 4-6学时 任务2：GmSSL 库编程

用 GmSSL 库（libgmssl）编程实现同一组算法调用，全部 PASS；SM3 与 HMAC-SM3 结果与 OpenSSL 实现逐字节一致，交叉验证两库正确性。

![](images/hours46-2-caimaojun_1.png)

![](images/hours46-2-caimaojun_2.png)

### 3.7 4-6学时 任务3/4：库编程数字信封

Alice 用 OpenSSL 库编写发送程序 alice_send，自验通过后发布 exchange/task3_lib/，Bob 已接收成功；Bob 用 GmSSL 库编写发送程序并发布 exchange/task4_lib/，Alice 用 GmSSL 库接收程序 alice_recv_gm 实弹接收成功，明文为"20241304 元泓鉴"。双向库版数字信封互通完成。

![](images/hours46-3-caimaojun_1.png)

![](images/hours46-4-caimaojun_1.png)

![](images/hours46-4-caimaojun_2.png)

## 四、实验结果与分析

1. 命令与库编程两条路径下，SM2（加解密、签名验签）、SM3（摘要、HMAC）、SM4（加解密）全部正确运行，往返验证 PASS。
2. 交叉验证：OpenSSL 与 GmSSL 计算的 SM3 摘要、HMAC-SM3 值逐字节一致（如 e66a3cfe… 与 d1ceec90…），说明两套实现符合同一国密标准。
3. 数字信封：命令版与库版双向（Alice→Bob、Bob→Alice）均验签、解密钥、解明文成功，协议流程与实现正确。
4. 性能：SM4-CBC 大块吞吐约 110 MB/s，SM3 在 8KB 块上约 320 MB/s（WSL2 单线程量级参考）。
5. 工程性：公钥交换、信封传递、协作沟通全部通过 gitee 仓库与 MAILBOX 留言完成，git 提交完整记录过程。

## 五、遇到的问题与解决

1. GmSSL 安装后库不可用（/usr/local/lib 不在 ldconfig 缓存）：配置 ld.so.conf.d 并 ldconfig，Makefile 加 -L 与 rpath。
2. GmSSL 命令语法差异：rand 用 -outlen、sm4_cbc 需显式 -pkcs7_padding、sm3_hmac 密钥至少 12 字节、sm2verify 用 -pubkey。
3. openssl dgst 不接受 -in 参数，待签名/验签文件须用位置参数。
4. OpenSSL C API：EC 密钥挂 SM2 曲线不走 SM2 算法，须生成 SM2 类型密钥；EVP 签名/加密的长度参数为 in/out 语义。
5. GmSSL C API 摘要收尾函数为 sm3_finish（非 sm3_final）。
6. 队友命令版信封未传 IV：利用已知明文恢复 IV（IV = D(K,C1) xor P1）后解密成功，并推动协议约定 IV 传递方式。
7. 双人并行推送多次被拒：以 git pull --rebase 解决，未丢失提交。

## 六、实验总结

本次实验完整走通"命令实践 → 库编程 → 密码协议实现"的路径，掌握了 openEuler 下 OpenSSL/GmSSL 的使用与开发方法，理解了数字信封协议中对称加密、非对称加密与数字签名的组合方式，并通过交叉验证与双向互通确认了实现正确性。过程中最大的收获在于对协议细节（IV 传递、数据格式、密钥类型与接口语义）的重视，以及共享仓库协作模式的实践。

![](images/git_log_1.png)