# 密码系统设计 实验一 嵌入式开发基础

小组与角色：

| 学号 | 姓名 | 角色 | 主要分工 |
|--------|--------|-------|---------|
| 20241328 | 蔡贸俊 | Alice | 1-3学时任务3发送方、任务4接收方；OpenSSL 命令与库编程实践 |
| 20241304 | 元泓鉴 | Bob | 1-3学时任务3接收方、任务4发送方；GmSSL 命令与库编程实践 |

实验环境：openEuler 24.03 LTS (WSL2)、OpenSSL 3.0.12、GmSSL 3.3.0-dev、gcc 12.3.1

## 目录结构
- docs/：提交用记录（hours1-3.md、hours4-6.md、问题与反思.md），由 docs/records/ 装配生成
- docs/records/：过程记录原件，命名 <task>-<姓名拼音>.md，如 task1-caimaojun.md
- code/：C 源码与 Makefile，子目录 openssl_sm / gmssl_sm / envelope_openssl / envelope_gmssl
- keys/：仅公钥（命名 keys/学号_拼音/算法_pub.pem），私钥只留各自本机
- exchange/：数字信封协议传递文件（C、KC、S1、envelope），通过 git push/pull 交换
- MAILBOX.md：追加式留言板，双方沟通通道

## 协作约定
1. 每完成一项 git commit 一次，message 格式 `feat(task序号-端): 描述`，如 `feat(task3-alice): OpenSSL 命令数字信封发送侧`
2. 私钥绝不入库，.gitignore 已拦截 *_priv.pem / *_sk.pem；commit 前用 git status 自查
3. 公钥交换与信封传递均走本仓库：pull 对方 pub 与 exchange/ 文件即完成交接
4. 过程记录写入 docs/records/ 自己的文件，命令与终端输出完整贴入代码块；提交前装配为 docs/hours1-3.md、hours4-6.md 并转 PDF
5. 蔡贸俊侧 commit 后自动 push；元泓鉴侧每完成一项手动 push；冲突时先 pull 解决后再 push

## 协作沟通（MAILBOX.md）
- 追加式留言板，格式 `### [日期] 发送人 → 接收人`，不改不删他人消息
- 每次操作前先 git pull 查看新留言与新文件；结论性内容必须落板

## 评分点自查
- [ ] 1-3学时 任务1/2/3/4 记录与 commit
- [ ] 4-6学时 任务1/2/3/4 记录与 commit
- [ ] git log 汇总（含两名成员的 commit）
- [ ] Markdown 转 PDF
- [ ] 实验报告 doc
