# 任务2：GmSSL 命令实践记录

## 实验人
20241304 元泓鉴

## 环境
WSL openEuler 24.03，GmSSL 3.3.0-dev.1183（源码编译安装至 ~/.local/gmssl）

## 操作内容

### SM3 摘要
```
gmssl sm3 plain.txt
```
计算结果与 OpenSSL sm3 一致，验证正确性。

### SM4-CBC 加解密
```
gmssl sm4_cbc -encrypt -key <key> -iv <iv> -pkcs7_padding -in plain.txt -out C.bin
gmssl sm4_cbc -decrypt -key <key> -iv <iv> -pkcs7_padding -in C.bin -out plain_recv.txt
```

### SM2 密钥生成
```
gmssl sm2keygen -pass 12345678 -out sm2_priv.pem -pubout sm2_pub.pem
```

### SM2 加解密
```
gmssl sm2encrypt -pubkey sm2_pub.pem -in k.bin -out KC.bin
gmssl sm2decrypt -pass 12345678 -inkey sm2_priv.pem -in KC.bin -out k_recv.bin
```

### SM2 签名验签
```
gmssl sm2sign -key sm2_priv.pem -pass 12345678 -in C.bin -out S1.bin
gmssl sm2verify -pubkey sm2_pub.pem -signature S1.bin -in C.bin
```

## 结果
全部操作成功，GmSSL 与 OpenSSL 的 SM3 摘要值完全一致，交叉验证通过。
