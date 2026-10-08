# 4-6学时 任务3 OpenSSL 库编程数字信封 Alice→Bob（蔡贸俊）

代码：code/envelope_ossl/alice_send.c + Makefile
用法：alice_send <我的私钥pem> <对方公钥pem> <输出目录>
本次运行：alice_send ~/work/task3/alice_openssl_priv.pem keys/20241304_yuanhongjian/sm2_openssl_pub.pem exchange/task3_lib

## 运行

```text
$ ./code/envelope_ossl/alice_send ~/work/task3/alice_openssl_priv.pem keys/20241304_yuanhongjian/sm2_openssl_pub.pem exchange/task3_lib
k = 83773f00767a2119a90ea6700e9239c4
C len = 32
KC len = 124
S1 len = 72
self verify: PASS
self decrypt: PASS
envelope len = 228, written to exchange/task3_lib
$ ls -l exchange/task3_lib/
C.bin 32 / envelope.bin 228 / KC.bin 124 / lens.txt 30 / S1.bin 72
```

## 实现说明
- 程序内完成协议全流程：RAND_bytes 生成 16 字节 k → EVP SM4-CBC 加密明文得 C → EVP_PKEY_encrypt（Bob 公钥）加密 k 得 KC → EVP_DigestSign（SM3，我的私钥）签名 C 得 S1 → 拼接 C||KC||S1 写 exchange/task3_lib/ 并附 lens.txt 长度表
- 发送前自验：用自己公钥验 S1、用 k 解 C 与明文比对，双 PASS 才发布
- 坑：PEM_read_PUBKEY 为 4 参数（fp, cb, u, pass），漏写第 4 个编译报 too few arguments
- Bob 接收侧与命令版步骤相同（验签→pkeyutl 解密 KC→sm4 解密 C），见 MAILBOX 通知

## 小结

OpenSSL 库编程实现带签名数字信封发送侧，自验通过，信封已发布 exchange/task3_lib/ 待 Bob 接收。
