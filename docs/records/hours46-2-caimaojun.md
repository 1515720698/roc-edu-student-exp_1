# 4-6学时 任务2 GmSSL 库编程实践（蔡贸俊）

环境：openEuler 24.03 LTS (WSL2)、GmSSL 3.3.0-dev.1183（libgmssl）
代码：code/gmssl_sm/gm_demo.c + Makefile

## 编译

```text
$ cd code/gmssl_sm && make
gcc -Wall -O2 -o gm_demo gm_demo.c -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lgmssl
```

## 运行

```text
$ ./gm_demo
== SM4-CBC (GmSSL lib) ==
key = d81254e31981eaa4dafc361368e10bf8
cipher = 44070f49684163b359df1d818bb664a9ad36a036dd7be2205bc7cc8afbe4a8ca1be36c03e89833c3dfefc7e
plain = Hello 密码系统设计 20241328 蔡贸俊
SM4 roundtrip: PASS
== SM3 / SM3-HMAC (GmSSL lib) ==
sm3 = e66a3cfe775153beefdbc3ffc5e30a78c0fe2b6fc43891ab9616abdb1962eaa6
hmac-sm3 = d1ceec90647b6dbb21f33624a8527b7d96f523bffffed7969e635185f13aeb1a
== SM2 sign/verify, encrypt/decrypt (GmSSL lib) ==
siglen = 71
SM2 verify: PASS
session key = ffb756a0bf538c1b5ea4a34b61c1327d
kc len = 123
SM2 key roundtrip: PASS
```

## 实现说明
- SM4-CBC：`SM4_CBC_CTX` + `sm4_cbc_encrypt_init/update/finish`（finish 内完成 PKCS#7 填充处理），解密对称
- SM3：`sm3_init/sm3_update/sm3_finish`；HMAC：`sm3_hmac_init/update/finish`
- SM2：`sm2_key_generate` 生成密钥；签名 `sm2_sign_init/update/finish`（ID 取 1234567812345678）；验签 `sm2_verify_init/update/finish`；加密解密 `sm2_encrypt/sm2_decrypt`
- 坑1：GmSSL 3 的摘要收尾函数名为 `sm3_finish`，误用 `sm3_final` 导致隐式声明与 undefined reference
- 坑2：`cmake --install` 后 libgmssl.so 位于 /usr/local/lib 且不在 ldconfig 缓存中，首次链接/运行失败；已写入 /etc/ld.so.conf.d/gmssl.conf 并 ldconfig，Makefile 同时带 -L 与 rpath
- 交叉验证：SM3 与 HMAC-SM3 值与 hours46-1 的 OpenSSL EVP 实现逐字节一致（e66a3cfe…/d1ceec90…），两套库互验正确

## 小结

GmSSL 库编程完成 SM2（加密解密、签名验签）、SM3（摘要、HMAC）、SM4（加密解密）调用，全部 PASS，并与 OpenSSL 实现交叉验证一致。
