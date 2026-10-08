# 任务4 GmSSL 命令数字信封 Bob→Alice

角色：Bob=20241304 元泓鉴（发送），Alice=20241328 蔡贸俊（接收）
密钥：Bob 公钥 keys/20241304_yuanhongjian/sm2_gmssl_pub.pem；Alice 私钥本机（GmSSL 格式）
传递文件：exchange/task4/（C.bin、KC.bin、S1.bin）

## Alice 接收侧

```text
$ gmssl sm2verify -pubkey keys/20241304_yuanhongjian/sm2_gmssl_pub.pem -in exchange/task4/C.bin -sig exchange/task4/S1.bin
verify : success
$ gmssl sm2decrypt -key alice_gmssl_priv.pem -pass ****** -in exchange/task4/KC.bin -out k4.bin
$ python3 -c '...hexlify...' < k4.bin
7867646ecd25d0ff82b00cee25975aae
$ gmssl sm4_cbc -decrypt -pkcs7_padding -key 7867646ecd25d0ff82b00cee25975aae -iv 00000000000000000000000000000000 -in exchange/task4/C.bin
（首块乱码，第二块出现合法 PKCS#7 填充 0x0d x13）
```

问题：全 0 IV 解密首块乱码，但第二块填充合法 → 密钥与 CBC+PKCS#7 模式正确，发送方使用了非零随机 IV 且未随信封传递。

处理（已知明文恢复 IV）：
- 第二块明文末 3 字节 0x89 0xB4 0x0a = "鉴"末两字节+换行 → 推断明文为"20241304 元泓鉴\n"共 19 字节，首块明文 P1 = 32 30 32 34 31 33 30 34 20 e5 85 83 e6 b3 93 e9
- CBC 性质：P1 = D(K,C1) xor IV，故 IV = D(K,C1) xor P1
- 无填充解密取首块 D(K,C1) = 95a57c1cbf2a25e7a2a434c6c3878775
- 得 IV = a7954e288e1915d38241b1452534149c

```text
$ gmssl sm4_cbc -decrypt -pkcs7_padding -key 7867646ecd25d0ff82b00cee25975aae -iv a7954e288e1915d38241b1452534149c -in exchange/task4/C.bin
20241304 元泓鉴
```

结论：Sm2Very(PKa,S1) 通过、Sm2Dec(SKa,KC)=k、Sm4Dec(k,C)=P="20241304 元泓鉴"，接收侧完成。
反思：数字信封协议应约定 IV 传递方式（随信封头部携带或固定全 0）；本次接收方以已知明文（对方学号姓名）恢复 IV，属协议设计教训，已留言请 Bob 补交 iv.bin。

## Bob 发送侧（元泓鉴填写）

（待填：k 生成、C=Sm4Enc(k,P)、KC=Sm2Enc(PKa,k)、S1=Sm2Sign(SKb,C) 的命令与输出，含所用 IV）

### Bob 发送侧（元泓鉴 20241304）

**密钥生成：**
```
$ gmssl rand -outlen 16 -hex
KEY=7867646ECD25D0FF82B00CEE25975AAE
IV=A7954E288E1915D38241B1452534149C
```

**SM4 加密明文：**
```
$ gmssl sm4_cbc -encrypt -key $KEY -iv $IV -pkcs7_padding -in plain.txt -out C.bin
C.bin = 32 bytes
```

**SM2 加密密钥（Alice公钥）：**
```
$ printf '\x78\x67\x64\x6e\xcd\x25\xd0\xff\x82\xb0\x0c\xee\x25\x97\x5a\xae' > k.bin
$ gmssl sm2encrypt -pubkey alice_pub.pem -in k.bin -out KC.bin
KC.bin = 122 bytes
```

**SM2 签名密文（Bob私钥）：**
```
$ gmssl sm2sign -key bob_priv.pem -pass 12345678 -in C.bin -out S1.bin
S1.bin = 72 bytes
```

**发送文件：**
C.bin、KC.bin、S1.bin 发布至 exchange/task4/。

**说明：** 首次发送时 IV 使用随机值且未随信封传递，后补交 exchange/task4/iv.bin（十六进制 A7954E288E1915D38241B1452534149C）。后续编程版信封统一使用全0 IV。
