# 4-6学时 任务1 OpenSSL 库编程实践（蔡贸俊）

环境：openEuler 24.03 LTS (WSL2)、OpenSSL 3.0.12（EVP API）
代码：code/openssl_sm/sm_demo.c + Makefile

## 编译

```text
$ cd code/openssl_sm && make
gcc -Wall -O2 -o sm_demo sm_demo.c -lcrypto
```

## 运行

```text
$ ./sm_demo
== SM4-CBC (EVP) ==
key = 78da5de71dc1cbe89898b1c2a9fb1198
cipher = e255744fdfbe034eb0f2a0ba37b21629d121588017407540a6f87758c8cc125b443a899bd7f0bd93cb6f397db72bb626
plain = Hello 密码系统设计 20241328 蔡贸俊
SM4 roundtrip: PASS
== SM3 digest / HMAC (EVP) ==
sm3 = e66a3cfe775153beefdbc3ffc5e30a78c0fe2b6fc43891ab9616abdb1962eaa6
hmac-sm3 = d1ceec90647b6dbb21f33624a8527b7d96f523bffffed7969e635185f13aeb1a
== SM2 sign/verify, encrypt/decrypt (EVP) ==
siglen = 70
SM2 verify: PASS
session key = 7db0c30b11543718dce0f18604192e68
kc len = 124
SM2 key roundtrip: PASS
```

## 实现说明
- SM4-CBC：`EVP_sm4_cbc()` + `EVP_CipherInit_ex/Update/Final`，密钥由 `RAND_bytes` 随机生成 16 字节，默认 PKCS#7 填充
- SM3：`EVP_DigestInit_ex(EVP_sm3())`；HMAC：`EVP_MAC_fetch("HMAC")` 后用 OSSL_PARAM 指定 digest=SM3
- SM2 密钥生成：`EVP_PKEY_CTX_new_from_name(NULL, "SM2", NULL)`；签名验签：`EVP_DigestSign/Verify` 系列配 SM3；加密解密：`EVP_PKEY_encrypt/decrypt`（密文为 ASN.1 DER，124 字节）
- 坑1：生成 EC 类型密钥挂 SM2 曲线不会走 SM2 算法路径，必须生成 SM2 类型密钥（首版 sign FAIL 的原因）
- 坑2：`EVP_DigestSignFinal`/`EVP_PKEY_encrypt` 的 `size_t` 长度参数是 in/out 参数，调用前必须置为缓冲区大小，传 0 直接失败
- 说明：此处 SM3 值与任务1 中 plain.txt 摘要不同，因本程序消息串不含末尾换行符

## 小结

OpenSSL 库编程完成 SM2（加密解密、签名验签）、SM3（摘要、HMAC）、SM4（加密解密）调用，全部往返 PASS。
