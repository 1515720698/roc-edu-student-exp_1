# 本次实验 git log 运行结果

命令：git log --date=iso --pretty=format:%h_%ad_%an_%s

```text
5b62817 2026-10-08 10:04:39 +0800 蔡贸俊 docs: 问题与反思记录，留言催促Bob补信封文件与记录
9a85423 2026-10-08 10:02:13 +0800 蔡贸俊 feat(hours46-4): GmSSL库编程数字信封接收侧程序完成，自测PASS
2f266f5 2026-10-08 09:59:18 +0800 蔡贸俊 feat(hours46-3): OpenSSL库编程数字信封发送侧完成并发布exchange/task3_lib
3b3cc65 2026-10-08 09:57:09 +0800 20241304-yuanhongjian Merge branch 'master' of gitee.com:xiaoyuanyuan999/roc-edu-student-exp_1
eebd252 2026-10-08 09:57:09 +0800 20241304-yuanhongjian feat(gmssl-envelope): 编程版数字信封收发程序，验签解密全部PASS
40efcf8 2026-10-08 09:54:12 +0800 蔡贸俊 feat(hours46-2): GmSSL库编程调用SM2/SM3/SM4含记录，留言请求Bob库版信封
f601d08 2026-10-08 09:54:02 +0800 20241304-yuanhongjian Merge branch 'master' of gitee.com:xiaoyuanyuan999/roc-edu-student-exp_1
db12ad4 2026-10-08 09:53:17 +0800 20241304-yuanhongjian feat(task3-bob): 真实信封接收成功，验签解密通过
18877f6 2026-10-08 09:46:45 +0800 蔡贸俊 feat(task4-alice): GmSSL数字信封接收侧验签解密成功，已知明文恢复缺失IV
6e5e8f2 2026-10-08 09:45:25 +0800 20241304-yuanhongjian Merge remote-tracking branch 'origin/master'
d120834 2026-10-08 09:44:28 +0800 20241304-yuanhongjian feat(gmssl): C库调用SM2/SM3/SM4全部PASS，含Makefile
21de1c6 2026-10-08 09:40:30 +0800 蔡贸俊 feat(task3-alice): OpenSSL命令数字信封发送侧 C||KC||S1 发布至exchange并留言通知Bob
86c455a 2026-10-08 09:40:10 +0800 20241304-yuanhongjian feat(task4-bob): GmSSL数字信封发送，C/KC/S1已发布
f721832 2026-10-08 09:35:00 +0800 蔡贸俊 feat(hours46-1): OpenSSL库编程调用SM2/SM3/SM4(EVP)含记录
3026097 2026-10-08 09:34:34 +0800 20241304-yuanhongjian feat(task3-bob): 发布Bob双格式SM2公钥
aa47719 2026-10-08 08:53:51 +0800 蔡贸俊 feat(task3-alice): 生成双格式SM2密钥对并发布公钥，更新密钥约定与留言板
7466538 2026-10-08 08:52:37 +0800 蔡贸俊 feat(task2): gmssl 命令实践-SM4/SM3/SM2命令与openssl交叉验证
bc0170a 2026-10-08 08:46:11 +0800 蔡贸俊 feat(task1): openssl 命令实践-speed性能测试与小结
a36696c 2026-10-08 08:44:56 +0800 蔡贸俊 feat(task1): openssl 命令实践-SM3摘要HMAC与SM2签名验签
d05e572 2026-10-08 08:43:47 +0800 蔡贸俊 feat(task1): openssl 命令实践-SM4对称加解密与随机数
467bedb 2026-10-08 08:42:12 +0800 蔡贸俊 chore: 仓库骨架、协作约定与 MAILBOX 留言板
fb18c87 2026-10-08 00:36:57 +0000 元泓鉴 Initial commit
```
