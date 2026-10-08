#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

static int write_file(const char *path, const unsigned char *d, size_t n)
{
    FILE *fp = fopen(path, "wb");
    if (!fp)
        return 0;
    fwrite(d, 1, n, fp);
    fclose(fp);
    return 1;
}

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

int main(int argc, char **argv)
{
    const char *msg = "蔡贸俊 20241328";
    int msglen = (int)strlen(msg);
    unsigned char k[16], iv[16] = {0};
    unsigned char C[256], KC[512], S1[512], P[256], env[1536];
    int clen = 0, plen = 0;
    size_t kclen = sizeof(KC), s1len = sizeof(S1);
    EVP_PKEY *mine = NULL, *peer = NULL;
    EVP_PKEY_CTX *pctx = NULL;
    EVP_MD_CTX *mctx = NULL, *vctx = NULL;
    FILE *fp = NULL;
    char path[512];
    size_t envlen = 0;

    if (argc != 4) {
        fprintf(stderr, "usage: %s my_priv.pem peer_pub.pem outdir\n", argv[0]);
        return 1;
    }
    fp = fopen(argv[1], "rb");
    if (fp) {
        mine = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
        fclose(fp);
    }
    fp = fopen(argv[2], "rb");
    if (fp) {
        peer = PEM_read_PUBKEY(fp, NULL, NULL, NULL);
        fclose(fp);
    }
    if (!mine || !peer) {
        fprintf(stderr, "key load fail\n");
        return 1;
    }

    RAND_bytes(k, sizeof(k));
    hexprint("k = ", k, 16);
    if (!sm4_cbc(k, iv, (const unsigned char *)msg, msglen, C, &clen, 1)) {
        fprintf(stderr, "SM4 enc fail\n");
        return 1;
    }
    printf("C len = %d\n", clen);

    pctx = EVP_PKEY_CTX_new(peer, NULL);
    if (!pctx || EVP_PKEY_encrypt_init(pctx) <= 0
        || EVP_PKEY_encrypt(pctx, KC, &kclen, k, sizeof(k)) <= 0) {
        fprintf(stderr, "SM2 enc fail\n");
        return 1;
    }
    printf("KC len = %zu\n", kclen);

    mctx = EVP_MD_CTX_new();
    if (!mctx || EVP_DigestSignInit(mctx, NULL, EVP_sm3(), NULL, mine) <= 0
        || EVP_DigestSignUpdate(mctx, C, clen) <= 0
        || EVP_DigestSignFinal(mctx, S1, &s1len) <= 0) {
        fprintf(stderr, "SM2 sign fail\n");
        return 1;
    }
    printf("S1 len = %zu\n", s1len);

    vctx = EVP_MD_CTX_new();
    EVP_DigestVerifyInit(vctx, NULL, EVP_sm3(), NULL, mine);
    EVP_DigestVerifyUpdate(vctx, C, clen);
    printf("self verify: %s\n", EVP_DigestVerifyFinal(vctx, S1, s1len) == 1 ? "PASS" : "FAIL");
    if (sm4_cbc(k, iv, C, clen, P, &plen, 0) && plen == msglen && memcmp(P, msg, msglen) == 0)
        printf("self decrypt: PASS\n");
    else
        printf("self decrypt: FAIL\n");

    memcpy(env, C, clen);
    memcpy(env + clen, KC, kclen);
    memcpy(env + clen + kclen, S1, s1len);
    envlen = clen + kclen + s1len;

    sprintf(path, "%s/C.bin", argv[3]);
    write_file(path, C, clen);
    sprintf(path, "%s/KC.bin", argv[3]);
    write_file(path, KC, kclen);
    sprintf(path, "%s/S1.bin", argv[3]);
    write_file(path, S1, s1len);
    sprintf(path, "%s/envelope.bin", argv[3]);
    write_file(path, env, envlen);
    sprintf(path, "%s/lens.txt", argv[3]);
    fp = fopen(path, "w");
    fprintf(fp, "C.bin %d\nKC.bin %zu\nS1.bin %zu\n", clen, kclen, s1len);
    fclose(fp);
    printf("envelope len = %zu, written to %s\n", envlen, argv[3]);
    EVP_PKEY_free(mine);
    EVP_PKEY_free(peer);
    return 0;
}
