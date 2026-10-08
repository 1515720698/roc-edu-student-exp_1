#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/rand.h>
#include <openssl/params.h>

static void hexprint(const char *tag, const unsigned char *d, int n)
{
    printf("%s", tag);
    for (int i = 0; i < n; i++)
        printf("%02x", d[i]);
    printf("\n");
}

static int sm4_cbc(const unsigned char *key, const unsigned char *iv,
                   const unsigned char *in, int inlen,
                   unsigned char *out, int *outlen, int enc)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len = 0, ok = 0;
    if (!ctx || !EVP_CipherInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv, enc))
        goto end;
    if (!EVP_CipherUpdate(ctx, out, &len, in, inlen))
        goto end;
    *outlen = len;
    if (!EVP_CipherFinal_ex(ctx, out + len, &len))
        goto end;
    *outlen += len;
    ok = 1;
end:
    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

static int sm3_digest(const unsigned char *in, int inlen, unsigned char *md)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    unsigned int n = 0;
    int ok = 0;
    if (ctx && EVP_DigestInit_ex(ctx, EVP_sm3(), NULL)
        && EVP_DigestUpdate(ctx, in, inlen)
        && EVP_DigestFinal_ex(ctx, md, &n))
        ok = 1;
    EVP_MD_CTX_free(ctx);
    return ok;
}

static int sm3_hmac(const unsigned char *key, int keylen,
                    const unsigned char *in, int inlen, unsigned char *mac)
{
    EVP_MAC *m = EVP_MAC_fetch(NULL, "HMAC", NULL);
    EVP_MAC_CTX *ctx = EVP_MAC_CTX_new(m);
    size_t n = 0;
    int ok = 0;
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_utf8_string("digest", "SM3", 0);
    params[1] = OSSL_PARAM_construct_end();
    if (ctx && EVP_MAC_init(ctx, key, keylen, params)
        && EVP_MAC_update(ctx, in, inlen)
        && EVP_MAC_final(ctx, mac, &n, 32))
        ok = 1;
    EVP_MAC_CTX_free(ctx);
    EVP_MAC_free(m);
    return ok;
}

static EVP_PKEY *sm2_keygen(void)
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_from_name(NULL, "SM2", NULL);
    EVP_PKEY *pkey = NULL;
    if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0)
        goto end;
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0)
        pkey = NULL;
end:
    EVP_PKEY_CTX_free(ctx);
    return pkey;
}

static int sm2_sign(EVP_PKEY *key, const unsigned char *in, int inlen,
                    unsigned char *sig, size_t *siglen)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    int ok = 0;
    if (ctx && EVP_DigestSignInit(ctx, NULL, EVP_sm3(), NULL, key) > 0
        && EVP_DigestSignUpdate(ctx, in, inlen) > 0
        && EVP_DigestSignFinal(ctx, sig, siglen) > 0)
        ok = 1;
    EVP_MD_CTX_free(ctx);
    return ok;
}

static int sm2_verify(EVP_PKEY *key, const unsigned char *in, int inlen,
                      const unsigned char *sig, size_t siglen)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    int ok = 0;
    if (ctx && EVP_DigestVerifyInit(ctx, NULL, EVP_sm3(), NULL, key) > 0
        && EVP_DigestVerifyUpdate(ctx, in, inlen) > 0
        && EVP_DigestVerifyFinal(ctx, sig, siglen) > 0)
        ok = 1;
    EVP_MD_CTX_free(ctx);
    return ok;
}

static int sm2_enc(EVP_PKEY *pub, const unsigned char *in, int inlen,
                   unsigned char *out, size_t *outlen)
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pub, NULL);
    int ok = 0;
    if (ctx && EVP_PKEY_encrypt_init(ctx) > 0
        && EVP_PKEY_encrypt(ctx, out, outlen, in, inlen) > 0)
        ok = 1;
    EVP_PKEY_CTX_free(ctx);
    return ok;
}

static int sm2_dec(EVP_PKEY *priv, const unsigned char *in, int inlen,
                   unsigned char *out, size_t *outlen)
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(priv, NULL);
    int ok = 0;
    if (ctx && EVP_PKEY_decrypt_init(ctx) > 0
        && EVP_PKEY_decrypt(ctx, out, outlen, in, inlen) > 0)
        ok = 1;
    EVP_PKEY_CTX_free(ctx);
    return ok;
}

int main(void)
{
    const char *msg = "Hello 密码系统设计 20241328 蔡贸俊";
    int msglen = (int)strlen(msg);
    unsigned char key[16], iv[16] = {0};
    unsigned char cipher[256], plain[256];
    int clen = 0, plen = 0;
    unsigned char md[32], mac[32];
    unsigned char sig[512], kt[16], kc[512], kback[512];
    size_t siglen = sizeof(sig), kclen = sizeof(kc), kblen = sizeof(kback);
    EVP_PKEY *pkey = NULL;

    printf("== SM4-CBC (EVP) ==\n");
    RAND_bytes(key, sizeof(key));
    hexprint("key = ", key, 16);
    if (!sm4_cbc(key, iv, (unsigned char *)msg, msglen, cipher, &clen, 1)) {
        printf("SM4 encrypt FAIL\n");
        return 1;
    }
    hexprint("cipher = ", cipher, clen);
    if (!sm4_cbc(key, iv, cipher, clen, plain, &plen, 0)) {
        printf("SM4 decrypt FAIL\n");
        return 1;
    }
    plain[plen] = 0;
    printf("plain = %s\n", plain);
    printf("SM4 roundtrip: %s\n", (plen == msglen && memcmp(plain, msg, msglen) == 0) ? "PASS" : "FAIL");

    printf("== SM3 digest / HMAC (EVP) ==\n");
    sm3_digest((unsigned char *)msg, msglen, md);
    hexprint("sm3 = ", md, 32);
    sm3_hmac((unsigned char *)"mykey123mykey123", 16, (unsigned char *)msg, msglen, mac);
    hexprint("hmac-sm3 = ", mac, 32);

    printf("== SM2 sign/verify, encrypt/decrypt (EVP) ==\n");
    pkey = sm2_keygen();
    if (!pkey) {
        printf("SM2 keygen FAIL\n");
        return 1;
    }
    if (!sm2_sign(pkey, (unsigned char *)msg, msglen, sig, &siglen)) {
        printf("SM2 sign FAIL\n");
        return 1;
    }
    printf("siglen = %zu\n", siglen);
    printf("SM2 verify: %s\n", sm2_verify(pkey, (unsigned char *)msg, msglen, sig, siglen) ? "PASS" : "FAIL");
    RAND_bytes(kt, sizeof(kt));
    hexprint("session key = ", kt, 16);
    if (!sm2_enc(pkey, kt, 16, kc, &kclen)) {
        printf("SM2 encrypt FAIL\n");
        return 1;
    }
    printf("kc len = %zu\n", kclen);
    if (!sm2_dec(pkey, kc, (int)kclen, kback, &kblen)) {
        printf("SM2 decrypt FAIL\n");
        return 1;
    }
    printf("SM2 key roundtrip: %s\n", (kblen == 16 && memcmp(kback, kt, 16) == 0) ? "PASS" : "FAIL");

    EVP_PKEY_free(pkey);
    return 0;
}
