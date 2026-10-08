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

### [2026-10-08] 元泓鉴 → 蔡贸俊
1. task3_lib 库版信封接收完成：验签 Verified OK，解密得到"蔡贸俊 20241328"。
2. task1/task2 记录已补写（docs/records/task1-yuanhongjian.md、task2-yuanhongjian.md）。
3. task3-envelope.md Bob 小节已补全，包含命令版和库版接收过程。
4. task4/iv.bin 已上传，task4_lib 编程版信封已发送。

### [2026-10-08] 元泓鉴 → 蔡贸俊
1. task4-envelope.md Bob 发送侧已补全，含命令和输出。
2. 已添加 .gitignore，编译产物从仓库移除。
3. task4_lib 编程版信封已发送，请接收验证。

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
全部收到，收尾确认：
1. task4_lib 库版信封我已接收成功（验签 PASS、明文"20241304 元泓鉴"），双向库版信封互通完成。
2. 你的 task1/task2 记录、task3 Bob 小节、task4/iv.bin 均已确认。
3. 我已装配 docs/hours1-3.md、hours4-6.md（含你的记录）并重新生成 PDF；请 pull 检查有无遗漏。
4. 剩余各自事项：实验报告 doc 各自按模板填写提交；提交前各自核对 README 评分点自查清单。

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
注意：你 08a606b 的 .gitignore 把源码（*.c、Makefile）也忽略并把 code/ 移出了仓库，这与"代码托管到 gitee"的评分要求冲突。我已修正 .gitignore（只忽略二进制与编译产物）并恢复全部源码入库，请 pull 后不要再次移除；你机器上的编译产物本就不会被新 .gitignore 跟踪。
