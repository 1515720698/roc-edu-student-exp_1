# MAILBOX 协作留言板

规则：只追加、不修改、不删除他人消息；每条格式如下：

### [日期] 发送人 → 接收人
内容

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
1. 仓库骨架与协作约定已建好，请先读 README.md。
2. 请在你自己机器上 clone 本仓库并完成首次 push（输入一次 gitee 密码），之后双方即可通过本仓库交换文件与消息。
3. 分工确认：你=Bob，负责任务3接收侧、任务4发送侧、GmSSL 命令与库编程主实践；我=Alice，负责 OpenSSL 侧。记录各自写 docs/records/ 下自己的文件。
4. 开始任务3前请生成你的 SM2 密钥对，公钥提交到 keys/20241304_yuanhongjian/sm2_pub.pem，私钥留你本机。
5. 有事就在本文件追加留言；双方约定每次操作前先 git pull 查看新留言与新文件。

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
1. 我的两个 SM2 公钥已发布：keys/20241328_caimaojun/sm2_openssl_pub.pem（任务3 OpenSSL 信封用）、sm2_gmssl_pub.pem（任务4 GmSSL 信封用）。OpenSSL 与 GmSSL 密钥格式不兼容，每人两种格式各生成一对，详见 keys/README.md。
2. 请按 keys/20241304_yuanhongjian/sm2_openssl_pub.pem、sm2_gmssl_pub.pem 发布你的两个公钥；我拿到 sm2_openssl_pub.pem 即可完成任务3发送侧，拿到 sm2_gmssl_pub.pem 后可以做任务4接收侧准备。
3. 任务1、任务2 我的记录已完成并推送（docs/records/task1-caimaojun.md、task2-caimaojun.md），请参考格式尽快补你的任务1/任务2 记录并 push。
4. 你完成任务4发送侧后，把信封文件放 exchange/task4/ 并留言通知我；我完成任务3发送侧后会放 exchange/task3/ 通知你。

### [2026-10-08] 元泓鉴 → 蔡贸俊
任务4 GmSSL 数字信封已发送，文件在 exchange/task4/（C.bin、KC.bin、S1.bin）。请验签并解密，明文为我的学号姓名。
