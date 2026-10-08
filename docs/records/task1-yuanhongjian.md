# 任务1：OpenSSL 命令实践记录

## 实验人
20241304 元泓鉴

## 环境
WSL openEuler 24.03，OpenSSL 3.0.12

## 操作内容

### SM3 摘要
```
openssl dgst -sm3 plain.txt
```
对明文文件计算 SM3 杂凑值，输出32字节摘要。

### HMAC-SM3
```
openssl dgst -sm3 -hmac "mykey123" plain.txt
```
使用密钥计算带消息认证码的 SM3 值。

### SM4-CBC 加解密
```
openssl rand -hex 16
openssl enc -sm4-cbc -e -K <key> -iv <iv> -in plain.txt -out enc.bin
openssl enc -sm4-cbc -d -K <key> -iv <iv> -in enc.bin
```
生成随机16字节密钥，使用 SM4-CBC 模式加解密，PKCS#7 填充。

### SM2 密钥生成
```
openssl ecparam -genkey -name SM2 -out sm2_priv.pem
openssl pkey -in sm2_priv.pem -pubout -out sm2_pub.pem
```

### SM2 加解密
```
openssl pkeyutl -encrypt -pubin -inkey sm2_pub.pem -in k.bin -out KC.bin
openssl pkeyutl -decrypt -inkey sm2_priv.pem -in KC.bin -out k_recv.bin
```

### SM2 签名验签
```
openssl dgst -sm3 -sign sm2_priv.pem -out sign.bin plain.txt
openssl dgst -sm3 -verify sm2_pub.pem -signature sign.bin plain.txt
```

## 结果
全部操作成功，加解密往返一致，验签输出 Verified OK。
