# keys 目录

只允许公钥入库，命名：keys/学号_拼音/算法_格式_pub.pem，例如：
- keys/20241328_caimaojun/sm2_openssl_pub.pem（OpenSSL 格式，任务3 用）
- keys/20241328_caimaojun/sm2_gmssl_pub.pem（GmSSL 格式，任务4 用）

OpenSSL 与 GmSSL 的 SM2 密钥格式不兼容，每人需生成两种格式各一对。
私钥仅存各自本机，.gitignore 已拦截 *_priv.pem 与 *_sk.pem；若误提交私钥，立即重新生成密钥对并清理历史。
