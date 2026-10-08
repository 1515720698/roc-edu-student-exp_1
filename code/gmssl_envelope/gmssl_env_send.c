#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmssl/sm2.h>
#include <gmssl/sm4.h>

static int rand_bytes(uint8_t *buf, size_t n)
{
    FILE *fp = fopen("/dev/urandom", "rb");
    if (!fp || fread(buf, 1, n, fp) != n) {
        if (fp) fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

static int load_priv(const char *path, const char *pass, SM2_KEY *key)
{
    FILE *fp = fopen(path, "rb");
    int ret;
    if (!fp) { perror(path); return -1; }
    ret = sm2_private_key_info_decrypt_from_pem(key, pass, fp);
    fclose(fp);
    return ret == 1 ? 0 : -1;
}

static int load_pub(const char *path, SM2_KEY *key)
{
    FILE *fp = fopen(path, "rb");
    int ret;
    if (!fp) { perror(path); return -1; }
    ret = sm2_public_key_info_from_pem(key, fp);
    fclose(fp);
    return ret == 1 ? 0 : -1;
}

static int read_file(const char *path, uint8_t *buf, size_t maxlen, size_t *outlen)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror(path); return -1; }
    *outlen = fread(buf, 1, maxlen, fp);
    fclose(fp);
    return 0;
}

static int write_file(const char *path, const uint8_t *buf, size_t len)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) { perror(path); return -1; }
    fwrite(buf, 1, len, fp);
    fclose(fp);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 6) {
        fprintf(stderr, "usage: %s <sender_priv.pem> <pass> <receiver_pub.pem> <plain.txt> <out_dir>\n", argv[0]);
        return 1;
    }
    const char *priv_path = argv[1];
    const char *pass = argv[2];
    const char *pub_path = argv[3];
    const char *plain_path = argv[4];
    const char *out_dir = argv[5];

    SM2_KEY sender, receiver;
    if (load_priv(priv_path, pass, &sender) != 0) {
        fprintf(stderr, "load sender private key FAIL\n");
        return 1;
    }
    if (load_pub(pub_path, &receiver) != 0) {
        fprintf(stderr, "load receiver public key FAIL\n");
        return 1;
    }

    uint8_t plain[1024];
    size_t plainlen;
    if (read_file(plain_path, plain, sizeof(plain), &plainlen) != 0)
        return 1;

    uint8_t sm4key[16], iv[16] = {0};
    if (rand_bytes(sm4key, 16) != 0) {
        fprintf(stderr, "rand FAIL\n");
        return 1;
    }

    SM4_KEY kenc;
    uint8_t C[2048];
    size_t Clen;
    sm4_set_encrypt_key(&kenc, sm4key);
    if (sm4_cbc_padding_encrypt(&kenc, iv, plain, plainlen, C, &Clen) != 1) {
        fprintf(stderr, "SM4 encrypt FAIL\n");
        return 1;
    }

    uint8_t KC[512];
    size_t KClen = sizeof(KC);
    if (sm2_encrypt(&receiver, sm4key, 16, KC, &KClen) != 1) {
        fprintf(stderr, "SM2 encrypt key FAIL\n");
        return 1;
    }

    SM2_SIGN_CTX sctx;
    uint8_t S1[SM2_MAX_SIGNATURE_SIZE];
    size_t S1len = sizeof(S1);
    sm2_sign_init(&sctx, &sender, SM2_DEFAULT_ID, SM2_DEFAULT_ID_LENGTH);
    sm2_sign_update(&sctx, C, Clen);
    if (sm2_sign_finish(&sctx, S1, &S1len) != 1) {
        fprintf(stderr, "SM2 sign FAIL\n");
        return 1;
    }

    char path[1024];
    snprintf(path, sizeof(path), "%s/C.bin", out_dir);
    write_file(path, C, Clen);
    snprintf(path, sizeof(path), "%s/KC.bin", out_dir);
    write_file(path, KC, KClen);
    snprintf(path, sizeof(path), "%s/S1.bin", out_dir);
    write_file(path, S1, S1len);

    printf("envelope sent to %s/\n", out_dir);
    printf("C.bin  %zu bytes\n", Clen);
    printf("KC.bin %zu bytes\n", KClen);
    printf("S1.bin %zu bytes\n", S1len);
    return 0;
}
