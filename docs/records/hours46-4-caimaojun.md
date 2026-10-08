# 4-6学时 任务4 GmSSL 库编程数字信封 Bob→Alice（蔡贸俊，接收侧）

代码：code/envelope_gmssl/alice_recv_gm.c + Makefile
用法：alice_recv_gm <我的GmSSL私钥pem> <口令> <对方GmSSL公钥pem> <信封目录>
接收流程：sm2_verify_init/update/finish 验签 S1 → sm2_decrypt 解 KC 得 k → sm4_cbc_decrypt_init/update/finish 解 C 得 P；iv.bin 存在则用之，否则全 0 IV。

## 自测（用自己密钥对模拟发送→接收，验证代码路径与 DER 格式处理）

```text
$ gmssl rand -hex -outlen 16 > k.hex; python3 ... < k.hex > k.bin
$ gmssl sm4_cbc -encrypt -pkcs7_padding -key $(cat k.hex) -iv 00000000000000000000000000000000 -in plain.txt -out C.bin
$ gmssl sm2sign -key alice_gmssl_priv.pem -pass ****** -in C.bin -out S1.bin
$ gmssl sm2encrypt -pubkey alice_gmssl_pub.pem -in k.bin -out KC.bin
$ alice_recv_gm alice_gmssl_priv.pem ****** alice_gmssl_pub.pem .
note: iv.bin absent, use zero IV
Sm2Very(PKb, S1): PASS
k len = 16
P = selftest 20241328
```

结论：验签、解密钥、解明文全链路 PASS；CLI 产生的 DER 签名/密文与库函数直接兼容，无需额外转换。

## 真实交换（待 Bob 库版信封）

等元泓鉴将库版信封（C.bin、KC.bin、S1.bin、iv.bin）发布到 exchange/task4_lib/ 后，运行：
alice_recv_gm <我的GmSSL私钥> <口令> keys/20241304_yuanhongjian/sm2_gmssl_pub.pem exchange/task4_lib
结果将追加到本文件。
