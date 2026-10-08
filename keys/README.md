# keys 目录

只允许公钥入库，命名：keys/学号_拼音/算法_pub.pem，例如 keys/20241328_caimaojun/sm2_pub.pem。
私钥仅存各自本机，.gitignore 已拦截 *_priv.pem 与 *_sk.pem；若误提交私钥，立即重新生成密钥对并清理历史。
