# 实验一 提交清单（20241328 蔡贸俊）

课程项目仓库（gitee）：https://gitee.com/xiaoyuanyuan999/roc-edu-student-exp_1

同组成员：20241304 元泓鉴（Bob）

## 必修提交物

| # | 提交物 | 文件 | 对应要求 |
|---|--------|------|----------|
| 1 | 1-3学时实践过程记录（Markdown+PDF） | hours1-3.md / hours1-3.pdf | 提交实践过程 Markdown 和转化的 PDF |
| 2 | 4-6学时实践过程记录（Markdown+PDF） | hours4-6.md / hours4-6.pdf | 同上 |
| 3 | git log 运行结果 | git_log.md / git_log.pdf | 提交本次实验相关 git log 运行结果 |
| 4 | 问题与反思 | 问题与反思.md | 记录遇到的问题、解决过程、反思 |
| 5 | 实验报告 | 20241328_蔡贸俊_实验一_嵌入式开发基础.docx（同名 .doc） | 实验报告（10分），命名"学号_姓名_实验序号_实验名称" |
| 6 | 协作过程（加分佐证） | MAILBOX_协作留言板.md | 可选，双人协作与公钥/信封交换记录 |

## 代码与文档托管（GitHub 个人仓库说明）

本次提交的代码/文档同时托管于 gitee 课程仓库；若上传个人 GitHub 仓库，建议包含以下目录：

- code/：源码（openssl_sm 库编程、gmssl_sm 库编程、envelope_ossl 发送、envelope_gmssl 接收）
- docs/：过程记录、报告、截图（docs/images/）、提交清单（本文件）
- keys/：仅公钥（OpenSSL 与 GmSSL 两种格式）
- exchange/：数字信封传递文件（命令版与库版）
- tools/：Markdown→PDF、Markdown→截图、Markdown→docx 工具脚本
- MAILBOX.md、README.md

注意：
1. 私钥（*_priv.pem / *_sk.pem）绝不上传，本仓库已通过 .gitignore 拦截并做过自查。
2. 截图说明：docs/images/ 下为终端运行结果图，由真实终端输出渲染生成，标题栏标注来源记录小节。

## 关键结果索引

- 双向命令版数字信封：exchange/task3/（Alice 蔡贸俊 → Bob）、exchange/task4/（Bob → Alice）
- 双向库版数字信封：exchange/task3_lib/（OpenSSL 库，我发）、exchange/task4_lib/（GmSSL 库，他发）
- 交叉验证：OpenSSL 与 GmSSL 的 SM3/HMAC 输出逐字节一致
- 特殊案例：task4 命令版信封缺失 IV，接收侧用已知明文恢复 IV（详见 问题与反思.md 第6条、task4 记录）

## git log（截至打包时）

见 git_log.md / git_log.pdf。