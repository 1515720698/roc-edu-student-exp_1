# MAILBOX 协作留言板

规则：只追加、不修改、不删除他人消息；每条格式如下：

### [日期] 发送人 → 接收人
内容

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
1. 仓库骨架与协作约定已建好，请先读 README.md。
2. 请在你自己机器上 clone 本仓库并完成首次 push，之后双方即可通过本仓库交换文件与消息。
3. 分工确认：你=Bob，负责任务3接收侧、任务4发送侧、GmSSL 命令与库编程主实践；我=Alice，负责 OpenSSL 侧。记录各自写 docs/records/ 下自己的文件。
4. 开始任务3前请生成你的 SM2 密钥对，公钥提交到 keys/20241304_yuanhongjian/sm2_pub.pem，私钥留你本机。
5. 有事就在本文件追加留言；双方约定每次操作前先 git pull 查看新留言与新文件。

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
1. 我的两个 SM2 公钥已发布：keys/20241328_caimaojun/sm2_openssl_pub.pem（任务3 OpenSSL 信封用）、sm2_gmssl_pub.pem（任务4 GmSSL 信封用）。
2. 请按 keys/20241304_yuanhongjian/ 发布你的两个公钥。
3. 任务1、任务2 我的记录已完成，请参考格式补你的记录。
4. 你完成任务4发送侧后，把信封文件放 exchange/task4/ 并留言通知我。

### [2026-10-08] 元泓鉴 → 蔡贸俊
任务4 GmSSL 数字信封已发送，文件在 exchange/task4/（C.bin、KC.bin、S1.bin）。请验签并解密，明文为我的学号姓名。

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
任务3 信封已发布到 exchange/task3/。请依次执行验签、解密KC、SM4解密三步。完成后把接收过程写入 docs/records/task3-envelope.md 的 Bob 小节并 push。

### [2026-10-08] 元泓鉴 → 蔡贸俊
任务3信封已成功接收：验签Verified OK，解密KC得到密钥，SM4解密得到"蔡贸俊 20241328"。

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
任务4 接收完成：验签 success，解密得"20241304 元泓鉴"。但你的 SM4 加密用了随机 IV 且未随信封传递，我用已知明文反推出 IV=a7954e288e1915d38241b1452534149c 后解密成功。请把真实 IV 提交为 exchange/task4/iv.bin。

另仍缺：你的任务1/任务2 记录、task3-envelope.md 的 Bob 接收侧记录。

---

### [2026-10-08] 蔡贸俊 → 元泓俊
1. 我的 GmSSL 库编程已完成（code/gmssl_sm/，全 PASS）。
2. 4-6学时任务4：请用 GmSSL 库编程发一次数字信封，文件放 exchange/task4_lib/（C.bin、KC.bin、S1.bin、iv.bin）。
3. 4-6学时任务3：我的 OpenSSL 库版数字信封已发布到 exchange/task3_lib/，请用你的 OpenSSL 私钥接收。
4. 小建议：编译产物不要入库，加 .gitignore。

---

### [2026-10-08] 元泓鉴 → 蔡贸俊
任务3 Bob 接收完成：验签 Verified OK，解密得到"蔡贸俊 20241328"。记录已写入 docs/records/task3-envelope.md。
