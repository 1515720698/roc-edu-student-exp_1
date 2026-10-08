# 任务2 GmSSL 命令实践（蔡贸俊）

环境：openEuler 24.03 LTS (WSL2)、GmSSL 3.3.0-dev.1183（源码编译安装）
工作目录：~/work/task2

## 1. 版本、随机数与 SM4-CBC 加解密

```text
$ gmssl version
GmSSL 3.3.0-dev.1183

$ gmssl rand -hex -outlen 16 | tee sm4.key
E11DD1C88E2355692465180DCFDD668C

$ gmssl sm4_cbc -encrypt -pkcs7_padding -key $(cat sm4.key) -iv 00000000000000000000000000000000 -in plain.txt -out plain.sm4
$ gmssl sm4_cbc -decrypt -pkcs7_padding -key $(cat sm4.key) -iv 00000000000000000000000000000000 -in plain.sm4 -out plain.dec
$ cmp plain.txt plain.dec && echo GMSSL-SM4-ROUNDTRIP-OK
GMSSL-SM4-ROUNDTRIP-OK

$ od -A x -t x1 plain.sm4 | head -2
000000 ed 45 f0 e3 20 53 84 75 4c c2 74 68 86 1a b1 88
000010 ec 87 c6 9f 48 9a 17 e6 dc 7b 09 0a fb 3e 57 2f
```

说明：GmSSL 的 rand 用 `-outlen` 指定字节数；sm4_cbc 对非 16 字节倍数明文必须显式加 `-pkcs7_padding`（首次运行报 input length must be multiple of 16 bytes，加该选项后通过）；密文 48 字节 = 45 字节明文 + 3 字节 PKCS#7 填充。

## 2. SM3 摘要、HMAC 与 OpenSSL 交叉验证

```text
$ gmssl sm3 -in plain.txt
4543fcd70da081d21d1310e973e4adcb94dce80b3cc6b21051c630740c913cc2

$ gmssl sm3 -in ../task1/plain2.txt
00841b73cac3992030440ade876be65bab0145dea1882506376d614477685c24

$ gmssl sm3_hmac -key 6d796b65793132336d796b6579313233 -in plain.txt
b8e7ce80387a45a853065ba2efc0b4024a7e95da8e1b36bf073d32c8ea1b67b7
```

说明：GmSSL 的 SM3 摘要与任务1中 OpenSSL `openssl dgst -sm3` 的结果逐字节一致（4543fcd7… 与 00841b73…），两套实现交叉验证正确；sm3_hmac 要求密钥至少 12 字节（首次用 8 字节密钥报 key should be at least 24 digits，改 16 字节后通过）。

## 3. SM2 密钥生成、签名验签、加密解密

```text
$ gmssl sm2keygen -pass 123456 -out sm2_priv.pem -pubout sm2_pub.pem
$ gmssl sm2sign -key sm2_priv.pem -pass 123456 -in plain.txt -out sig.bin
$ gmssl sm2verify -pubkey sm2_pub.pem -in plain.txt -sig sig.bin
verify : success
$ gmssl sm2encrypt -pubkey sm2_pub.pem -in plain.txt -out sm2.enc
$ gmssl sm2decrypt -key sm2_priv.pem -pass 123456 -in sm2.enc -out sm2.dec
$ cmp plain.txt sm2.dec && echo GMSSL-SM2-ENC-ROUNDTRIP-OK
GMSSL-SM2-ENC-ROUNDTRIP-OK
```

说明：GmSSL 私钥以口令加密保存（签名/解密需 -pass）；sm2verify 的公钥参数为 `-pubkey`（首次误用 -pkey 报错，查 usage 后修正）；签名验签、加密解密均通过。

## 小结

完成 GmSSL 命令实践：SM4-CBC 加解密、SM3 摘要/HMAC、SM2 密钥生成/签名验签/加密解密，并与 OpenSSL 的 SM3 摘要交叉验证一致。踩坑两条（sm4_cbc 需显式 -pkcs7_padding、sm3_hmac 密钥长度下限）已记入问题与反思素材。
