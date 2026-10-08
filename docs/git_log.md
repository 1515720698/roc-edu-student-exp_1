# 本次实验 git log 运行结果

命令：git log --date=iso --pretty=format:%h_%ad_%an_%s

```text
0ece6fb 2026-10-08 10:20:46 +0800 蔡贸俊 restore: 修正.gitignore误忽略源码，恢复code/全部源码与Makefile入库
d8ec42f 2026-10-08 10:17:26 +0800 蔡贸俊 docs: 纳入Bob记录重新装配提交稿与PDF，hours46-4补真实交换结果，收尾留言
05a64a7 2026-10-08 10:16:28 +0800 20241304-yuanhongjian Merge remote-tracking branch 'origin/master'
08a606b 2026-10-08 10:16:16 +0800 20241304-yuanhongjian docs: 补全task4 Bob发送侧记录，添加.gitignore移除编译产物
cf0e578 2026-10-08 10:14:12 +0800 蔡贸俊 docs: 装配hours1-3/hours4-6/git_log提交稿并生成中文PDF（嵌入uming字体），附md2pdf工具
f9e5f9d 2026-10-08 10:12:11 +0800 20241304-yuanhongjian docs: 补全task1/task2记录和task3-envelope Bob接收侧，留言通知
e123636 2026-10-08 10:09:40 +0800 20241304-yuanhongjian feat: 上传task4命令版IV和task4_lib编程版GmSSL信封
07bde16 2026-10-08 10:07:28 +0800 20241304-yuanhongjian merge: 解决MAILBOX冲突，保留双方留言
5b62817 2026-10-08 10:04:39 +0800 蔡贸俊 docs: 问题与反思记录，留言催促Bob补信封文件与记录
6c50644 2026-10-08 10:02:21 +0800 20241304-yuanhongjian feat(task3-bob): 真实接收OpenSSL数字信封，验签解密成功
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
