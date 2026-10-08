#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <gmssl/sm3.h>
#include <gmssl/sm4.h>
#include <gmssl/sm2.h>

static void hexprint(const char *tag, const uint8_t *d, size_t n)
{
    printf("%s", tag);
    for (size_t i = 0; i < n; i++)
        printf("%02x", d[i]);
    printf("\n");
}

static void test_sm3(const uint8_t *msg, size_t msglen)
{
    SM3_CTX ctx;
    uint8_t dgst[SM3_DIGEST_SIZE];

    printf("== SM3 ==\n");
    sm3_init(&ctx);
    sm3_update(&ctx, msg, msglen);
    sm3_finish(&ctx, dgst);
    hexprint("sm3 = ", dgst, 32);
}

static void test_hmac_sm3(const uint8_t *msg, size_t msglen)
{
    SM3_HMAC_CTX ctx;
    uint8_t mac[SM3_HMAC_SIZE];
    const uint8_t key[16] = "mykey123mykey123";

    printf("== HMAC-SM3 ==\n");
    sm3_hmac_init(&ctx, key, 16);
    sm3_hmac_update(&ctx, msg, msglen);
    sm3_hmac_finish(&ctx, mac);
    hexprint("hmac = ", mac, 32);
}

static void test_sm4_cbc(const uint8_t *msg, size_t msglen)
{
    SM4_KEY sm4key;
    uint8_t key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,
                       0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
    uint8_t iv[16] = {0};
    uint8_t enc[256], dec[256];
    size_t enclen = 0, declen = 0;

    printf("== SM4-CBC ==\n");
    sm4_set_encrypt_key(&sm4key, key);
    if (sm4_cbc_padding_encrypt(&sm4key, iv, msg, msglen, enc, &enclen) != 1) {
        printf("SM4 encrypt FAIL\n");
        return;
    }
    hexprint("cipher = ", enc, enclen);

    sm4_set_decrypt_key(&sm4key, key);
    if (sm4_cbc_padding_decrypt(&sm4key, iv, enc, enclen, dec, &declen) != 1) {
        printf("SM4 decrypt FAIL\n");
        return;
    }
    dec[declen] = 0;
    printf("plain = %s\n", dec);
    printf("SM4 roundtrip: %s\n",
        (declen == msglen && memcmp(dec, msg, msglen) == 0) ? "PASS" : "FAIL");
}

static void test_sm2(const uint8_t *msg, size_t msglen)
{
    SM2_KEY key;
    uint8_t enc[512], dec[256];
    size_t enclen = sizeof(enc), declen = sizeof(dec);
    uint8_t sig[SM2_MAX_SIGNATURE_SIZE];
    size_t siglen = sizeof(sig);
    SM2_SIGN_CTX sctx;
    SM2_VERIFY_CTX vctx;

    printf("== SM2 ==\n");
    if (sm2_key_generate(&key) != 1) {
        printf("SM2 keygen FAIL\n");
        return;
    }
    printf("SM2 keygen OK\n");

    if (sm2_encrypt(&key, msg, msglen, enc, &enclen) != 1) {
        printf("SM2 encrypt FAIL\n");
        return;
    }
    hexprint("sm2 enc = ", enc, enclen);

    if (sm2_decrypt(&key, enc, enclen, dec, &declen) != 1) {
        printf("SM2 decrypt FAIL\n");
        return;
    }
    dec[declen] = 0;
    printf("sm2 dec = %s\n", dec);
    printf("SM2 enc roundtrip: %s\n",
        (declen == msglen && memcmp(dec, msg, msglen) == 0) ? "PASS" : "FAIL");

    sm2_sign_init(&sctx, &key, SM2_DEFAULT_ID, SM2_DEFAULT_ID_LENGTH);
    sm2_sign_update(&sctx, msg, msglen);
    sm2_sign_finish(&sctx, sig, &siglen);
    hexprint("sig = ", sig, siglen);

    sm2_verify_init(&vctx, &key, SM2_DEFAULT_ID, SM2_DEFAULT_ID_LENGTH);
    sm2_verify_update(&vctx, msg, msglen);
    printf("SM2 verify: %s\n",
        sm2_verify_finish(&vctx, sig, siglen) == 1 ? "PASS" : "FAIL");
}

int main(void)
{
    const char *msg = "20241304 yuanhongjian";
    size_t msglen = strlen(msg);

    printf("msg = %s\n\n", msg);
    test_sm3((const uint8_t *)msg, msglen);
    printf("\n");
    test_hmac_sm3((const uint8_t *)msg, msglen);
    printf("\n");
    test_sm4_cbc((const uint8_t *)msg, msglen);
    printf("\n");
    test_sm2((const uint8_t *)msg, msglen);

    return 0;
}
