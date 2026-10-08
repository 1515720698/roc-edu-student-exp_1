#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmssl/sm2.h>
#include <gmssl/sm4.h>

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

int main(int argc, char **argv)
{
    if (argc != 5) {
        fprintf(stderr, "usage: %s <receiver_priv.pem> <pass> <sender_pub.pem> <in_dir>\n", argv[0]);
        return 1;
    }
    const char *priv_path = argv[1];
    const char *pass = argv[2];
    const char *pub_path = argv[3];
    const char *in_dir = argv[4];

    SM2_KEY receiver, sender;
    if (load_priv(priv_path, pass, &receiver) != 0) {
        fprintf(stderr, "load receiver private key FAIL\n");
        return 1;
    }
    if (load_pub(pub_path, &sender) != 0) {
        fprintf(stderr, "load sender public key FAIL\n");
        return 1;
    }

    char path[1024];
    uint8_t C[2048], KC[512], S1[SM2_MAX_SIGNATURE_SIZE];
    size_t Clen, KClen, S1len;

    snprintf(path, sizeof(path), "%s/C.bin", in_dir);
    if (read_file(path, C, sizeof(C), &Clen) != 0) return 1;
    snprintf(path, sizeof(path), "%s/KC.bin", in_dir);
    if (read_file(path, KC, sizeof(KC), &KClen) != 0) return 1;
    snprintf(path, sizeof(path), "%s/S1.bin", in_dir);
    if (read_file(path, S1, sizeof(S1), &S1len) != 0) return 1;

    SM2_VERIFY_CTX vctx;
    sm2_verify_init(&vctx, &sender, SM2_DEFAULT_ID, SM2_DEFAULT_ID_LENGTH);
    sm2_verify_update(&vctx, C, Clen);
    if (sm2_verify_finish(&vctx, S1, S1len) != 1) {
        printf("verify: FAIL\n");
        return 1;
    }
    printf("verify: PASS\n");

    uint8_t sm4key[16];
    size_t sm4keylen = sizeof(sm4key);
    if (sm2_decrypt(&receiver, KC, KClen, sm4key, &sm4keylen) != 1) {
        printf("SM2 decrypt key FAIL\n");
        return 1;
    }
    if (sm4keylen != 16) {
        printf("recovered key length %zu != 16\n", sm4keylen);
        return 1;
    }
    printf("session key recovered\n");

    SM4_KEY kdec;
    uint8_t iv[16] = {0};
    uint8_t plain[2048];
    size_t plainlen;
    sm4_set_decrypt_key(&kdec, sm4key);
    if (sm4_cbc_padding_decrypt(&kdec, iv, C, Clen, plain, &plainlen) != 1) {
        printf("SM4 decrypt FAIL\n");
        return 1;
    }
    plain[plainlen] = 0;
    printf("plain = %s\n", plain);
    return 0;
}
