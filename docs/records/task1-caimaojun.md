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

说明：明文为姓名+学号；SM4 密钥用 `openssl rand -hex 16` 产生 16 字节；IV 为 16 字节全 0（演示用）；加解密往返逐字节一致，密文为二进制无乱码差异。
