# 任务1 OpenSSL 命令实践（蔡贸俊）

环境：openEuler 24.03 LTS (WSL2)、OpenSSL 3.0.12
工作目录：~/work/task1（实践产物不入仓库，仅记录入库）

## 1. 版本查询、随机数与 SM4-CBC 对称加解密

```text
$ openssl version -a | head -4
OpenSSL 3.0.12 24 Oct 2023 (Library: OpenSSL 3.0.12 24 Oct 2023)
built on: Tue Apr 21 09:41:41 2026 UTC
platform: linux-x86_64
options:  bn(64,64)

$ printf 'Hello 密码系统设计 20241328 蔡贸俊\n' > plain.txt
$ openssl rand -hex 16 | tee sm4.key
25d20bb20a3d1a6d355f1a30959d4c73

$ openssl enc -sm4-cbc -e -in plain.txt -out plain.sm4 -K $(cat sm4.key) -iv 00000000000000000000000000000000
$ openssl enc -sm4-cbc -d -in plain.sm4 -out plain.dec -K $(cat sm4.key) -iv 00000000000000000000000000000000
$ cmp plain.txt plain.dec && echo SM4-CBC-ROUNDTRIP-OK
SM4-CBC-ROUNDTRIP-OK

$ od -A x -t x1 plain.sm4 | head -2
000000 69 0a b7 3d 8c f6 3a 34 77 90 d4 88 3f ea 2c c9
000010 c2 ad b4 64 14 0e 12 bb b8 95 61 10 b2 f7 09 a4
```

说明：明文为姓名+学号；SM4 密钥用 `openssl rand -hex 16` 产生 16 字节；IV 为 16 字节全 0（演示用）；加解密往返逐字节一致。

## 2. SM3 摘要与 HMAC

```text
$ openssl dgst -sm3 plain.txt
SM3(plain.txt)= 4543fcd70da081d21d1310e973e4adcb94dce80b3cc6b21051c630740c913cc2

$ openssl dgst -sm3 -hmac mykey123 plain.txt
HMAC-SM3(plain.txt)= 6dfb23c9f1d0f62fe7008353a13b4326c7187f39e84ae542d8204a8a487abbc9

$ { printf 'X'; tail -c +2 plain.txt; } > plain2.txt   # 仅改第1字节 H->X
$ cmp -l plain.txt plain2.txt | head -1
1 110 130
$ openssl dgst -sm3 plain2.txt
SM3(plain2.txt)= 00841b73cac3992030440ade876be65bab0145dea1882506376d614477685c24
```

说明：仅改动 1 个字节后摘要值完全改变，验证 SM3 雪崩效应；HMAC-SM3 以密钥 mykey123 做消息认证码。

## 3. SM2 密钥生成、签名与验签

```text
$ openssl ecparam -list_curves | grep -i sm2
SM2       : SM2 curve over a 256 bit prime field
$ openssl ecparam -genkey -name SM2 -out sm2_priv.pem
$ openssl ec -in sm2_priv.pem -pubout -out sm2_pub.pem
read EC key
-----BEGIN PUBLIC KEY-----
MFowFAYIKoEcz1UBgi0GCCqBHM9VAYItA0IABDFOerAAQhZwhNr2dJ3sPkVxTpj4
（以下略）
writing EC key
$ openssl dgst -sm3 -sign sm2_priv.pem -out sig.bin plain.txt
$ openssl dgst -sm3 -verify sm2_pub.pem -signature sig.bin plain.txt
Verified OK
```

说明：OpenSSL 3.0 原生支持 SM2 曲线；SM2 签名默认以 SM3 为摘要算法（签名者 ID 取默认值 1234567812345678）；验签输出 Verified OK。sm2_priv.pem 仅存实践目录，未入库。
