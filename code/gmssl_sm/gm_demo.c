#include <stdio.h>
#include <string.h>
#include <gmssl/sm4.h>
#include <gmssl/sm3.h>
#include <gmssl/sm2.h>
#include <gmssl/rand.h>

static void hexprint(const char *tag, const unsigned char *d, int n)
{
    printf("%s", tag);
    for (int i = 0; i < n; i++)
        printf("%02x", d[i]);
    printf("\n");
}

int main(void)
{
    const char *msg = "Hello 密码系统设计 20241328 蔡贸俊";
    size_t msglen = strlen(msg);
    uint8_t key[SM4_KEY_SIZE], iv[SM4_BLOCK_SIZE] = {0};
    uint8_t buf[512], plain[512];
    size_t outlen = 0, clen = 0, plen = 0;
    uint8_t md[SM3_DIGEST_SIZE], mac[SM3_HMAC_SIZE];
    SM4_CBC_CTX cctx;
    SM3_CTX sctx;
    SM3_HMAC_CTX hctx;
    SM2_KEY sk;
    SM2_SIGN_CTX sc;
    SM2_VERIFY_CTX vc;
    uint8_t sig[SM2_MAX_SIGNATURE_SIZE], kt[16], kc[512], kback[512];
    size_t siglen = 0, kclen = 0, kblen = 0;
    const char *id = "1234567812345678";

    printf("== SM4-CBC (GmSSL lib) ==\n");
    rand_bytes(key, sizeof(key));
    hexprint("key = ", key, 16);
    sm4_cbc_encrypt_init(&cctx, key, iv);
    sm4_cbc_encrypt_update(&cctx, (const uint8_t *)msg, msglen, buf, &outlen);
    clen = outlen;
    sm4_cbc_encrypt_finish(&cctx, buf + clen, &outlen);
    clen += outlen;
    hexprint("cipher = ", buf, (int)clen);
    sm4_cbc_decrypt_init(&cctx, key, iv);
    sm4_cbc_decrypt_update(&cctx, buf, clen, plain, &outlen);
    plen = outlen;
    sm4_cbc_decrypt_finish(&cctx, plain + plen, &outlen);
    plen += outlen;
    plain[plen] = 0;
    printf("plain = %s\n", plain);
    printf("SM4 roundtrip: %s\n", (plen == msglen && memcmp(plain, msg, msglen) == 0) ? "PASS" : "FAIL");

    printf("== SM3 / SM3-HMAC (GmSSL lib) ==\n");
    sm3_init(&sctx);
    sm3_update(&sctx, (const uint8_t *)msg, msglen);
    sm3_finish(&sctx, md);
    hexprint("sm3 = ", md, 32);
    sm3_hmac_init(&hctx, (const uint8_t *)"mykey123mykey123", 16);
    sm3_hmac_update(&hctx, (const uint8_t *)msg, msglen);
    sm3_hmac_finish(&hctx, mac);
    hexprint("hmac-sm3 = ", mac, 32);

    printf("== SM2 sign/verify, encrypt/decrypt (GmSSL lib) ==\n");
    sm2_key_generate(&sk);
    sm2_sign_init(&sc, &sk, id, strlen(id));
    sm2_sign_update(&sc, (const uint8_t *)msg, msglen);
    if (sm2_sign_finish(&sc, sig, &siglen) != 1) {
        printf("SM2 sign FAIL\n");
        return 1;
    }
    printf("siglen = %zu\n", siglen);
    sm2_verify_init(&vc, &sk, id, strlen(id));
    sm2_verify_update(&vc, (const uint8_t *)msg, msglen);
    printf("SM2 verify: %s\n", sm2_verify_finish(&vc, sig, siglen) == 1 ? "PASS" : "FAIL");
    rand_bytes(kt, sizeof(kt));
    hexprint("session key = ", kt, 16);
    if (sm2_encrypt(&sk, kt, sizeof(kt), kc, &kclen) != 1) {
        printf("SM2 encrypt FAIL\n");
        return 1;
    }
    printf("kc len = %zu\n", kclen);
    if (sm2_decrypt(&sk, kc, kclen, kback, &kblen) != 1) {
        printf("SM2 decrypt FAIL\n");
        return 1;
    }
    printf("SM2 key roundtrip: %s\n", (kblen == sizeof(kt) && memcmp(kback, kt, kblen) == 0) ? "PASS" : "FAIL");
    return 0;
}
