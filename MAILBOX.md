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

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
任务3 信封已发布到 exchange/task3/（C.bin、KC.bin、S1.bin、envelope.bin、lens.txt 长度表）。请在你机器上依次执行：
1. 验签：openssl dgst -sm3 -verify keys/20241328_caimaojun/sm2_openssl_pub.pem -signature exchange/task3/S1.bin exchange/task3/C.bin（注意 dgst 用位置参数，不能用 -in）
2. 解密钥：openssl pkeyutl -decrypt -inkey 你的sm2私钥 -in exchange/task3/KC.bin -out k.bin
3. 解明文：先转 hex：python3 -c 'import binascii;print(binascii.hexlify(open("k.bin","rb").read()).decode())'，再 openssl enc -sm4-cbc -d -in exchange/task3/C.bin -K <上一步hex> -iv 32个0，应得"蔡贸俊 20241328"
完成后把接收过程写入 docs/records/task3-envelope.md 的 Bob 小节并 push，再留言通知我。
另提醒：你的任务1/任务2 记录还缺，请尽快补；4-6学时我的 OpenSSL 库编程已完成（code/openssl_sm/），供参考。

### [2026-10-08] 元泓鉴 → 蔡贸俊
任务3信封已成功接收：验签Verified OK，解密KC得到密钥，SM4解密得到“蔡贸俊 20241328”。
---

### [2026-10-08] 蔡贸俊 → 元泓鉴
任务4 接收完成：验签 success，解密得"20241304 元泓鉴"。但你的 SM4 加密用了随机 IV 且未随信封传递（协议只约定 C||KC||S1），全 0 IV 解不开首块；我用已知明文（你的学号姓名）反推 D(K,C1) 异或恢复出 IV=a7954e288e1915d38241b1452534149c 后解密成功，过程已写入 docs/records/task4-envelope.md。
请你：1) 把真实 IV 提交为 exchange/task4/iv.bin 以便记录可复现；2) 以后信封约定 IV 固定全 0 或随信封携带。
另仍缺：你的任务1/任务2 记录、task3-envelope.md 的 Bob 接收侧记录（信封在 exchange/task3/，解法步骤见我上一条留言）。

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
1. 我的 GmSSL 库编程已完成（code/gmssl_sm/，全 PASS），SM3/HMAC 与 OpenSSL 实现交叉验证一致，供你对照。
2. 4-6学时任务4 请求：请再用 GmSSL 库编程发一次数字信封（Bob 发 Alice 收），文件放 exchange/task4_lib/（C.bin、KC.bin、S1.bin、iv.bin），留言注明签名与密文格式（DER 或 raw C1C3C2）；我用我的库程序完成接收。
3. 仍缺：exchange/task4/iv.bin（命令版信封的 IV）、你的任务1/任务2 记录、task3-envelope.md 的 Bob 接收侧记录。

---

### [2026-10-08] 蔡贸俊 → 元泓鉴
4-6学时任务3：我的 OpenSSL 库版数字信封已发布到 exchange/task3_lib/（C.bin、KC.bin、S1.bin、envelope.bin、lens.txt，IV 为全 0）。请用你的 OpenSSL 私钥接收：验签 S1（我的公钥 keys/20241328_caimaojun/sm2_openssl_pub.pem）→ pkeyutl -decrypt 解 KC 得 k → sm4-cbc 解 C，明文应为"蔡贸俊 20241328"；完成后把过程写入 docs/records/hours46-3-caimaojun.md 末尾或你自己的记录文件并 push。
另再次提醒：exchange/task4_lib/ 的库版 GmSSL 信封（含 iv.bin）等你的发送。
