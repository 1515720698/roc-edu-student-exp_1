# 任务3 OpenSSL 命令数字信封 Alice→Bob

角色：Alice=20241328 蔡贸俊（发送），Bob=20241304 元泓鉴（接收）
密钥：Alice 公钥 keys/20241328_caimaojun/sm2_openssl_pub.pem（私钥本机）；Bob 公钥 keys/20241304_yuanhongjian/sm2_openssl_pub.pem
传递文件：exchange/task3/（C.bin、KC.bin、S1.bin、envelope.bin、lens.txt）

## Alice 发送侧

```text
$ printf '蔡贸俊 20241328' > plain.txt
$ gmssl rand -hex -outlen 16 | tr -d '\n' | tr 'A-F' 'a-f' > k.hex
093f3fc30861d9daa78415b1ddb49c58
$ python3 -c 'import binascii,sys;sys.stdout.buffer.write(binascii.unhexlify(sys.stdin.read().strip()))' < k.hex > k.bin
$ openssl enc -sm4-cbc -e -in plain.txt -out C.bin -K $(cat k.hex) -iv 00000000000000000000000000000000
$ openssl pkeyutl -encrypt -pubin -inkey keys/20241304_yuanhongjian/sm2_openssl_pub.pem -in k.bin -out KC.bin
$ openssl dgst -sm3 -sign alice_openssl_priv.pem -out S1.bin C.bin
$ openssl dgst -sm3 -verify alice_openssl_pub.pem -signature S1.bin C.bin
Verified OK
$ openssl enc -sm4-cbc -d -in C.bin -out selfcheck.txt -K $(cat k.hex) -iv 00000000000000000000000000000000
$ cmp plain.txt selfcheck.txt && echo SELF-CHECK-OK
SELF-CHECK-OK
$ cat C.bin KC.bin S1.bin > envelope.bin
$ stat -c '%n %s' C.bin KC.bin S1.bin
C.bin 32
KC.bin 122
S1.bin 71
```

协议对应：k=gmssl rand 16字节；C=Sm4Enc(k,P)；KC=Sm2Enc(PKb,k)；S1=Sm2Sign(SKa,C)；数字信封=C||KC||S1。
发送侧自验：用自己的公钥验 S1 通过、用 k 解 C 与明文逐字节一致后才发布。
坑：openssl dgst 不接受 -in 参数，待签名/验签文件必须用位置参数给出（误用 -in 报 Can only sign or verify one file）。

## Bob 接收侧（元泓鉴填写）

（待填：Sm2Very(PKa,S1) → Sm2Dec(SKb,KC)=k → Sm4Dec(k,C)=P，明文应为"蔡贸俊 20241328"）

## Bob 接收侧（元泓鉴 20241304）

### 环境
- WSL openEuler 24.03，OpenSSL 3.0.12
- Bob 私钥：openssl-cmd/sm2_priv.pem
- Alice 公钥：keys/20241328_caimaojun/sm2_openssl_pub.pem

### 接收三步

**1. 验签**
```
openssl dgst -sm3 -verify sm2_openssl_pub.pem -signature S1.bin C.bin
```
结果：`Verified OK`

**2. SM2 解密 KC**
```
openssl pkeyutl -decrypt -inkey sm2_priv.pem -in KC.bin -out k.bin
```
恢复的 SM4 密钥（hex）：`093f3fc30861d9daa78415b1ddb49c58`（16字节）

**3. SM4-CBC 解密 C**
```
openssl enc -sm4-cbc -d -in C.bin -K 093f3fc30861d9daa78415b1ddb49c58 -iv 00000000000000000000000000000000
```
明文：`蔡贸俊 20241328`

### 结论
数字信封接收端完整跑通：验签通过、密钥恢复正确、SM4 解密得到预期明文。
