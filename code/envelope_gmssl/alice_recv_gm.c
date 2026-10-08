#include <stdio.h>
#include <string.h>
#include <gmssl/sm2.h>
#include <gmssl/sm3.h>
#include <gmssl/sm4.h>

static int read_file(const char *path, uint8_t *buf, size_t maxlen, size_t *len)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
        return 0;
    *len = fread(buf, 1, maxlen, fp);
    fclose(fp);
    return 1;
}

int main(int argc, char **argv)
{
    const char *id = "1234567812345678";
    SM2_KEY mine, peer;
    SM2_VERIFY_CTX vc;
    uint8_t C[512], KC[512], S1[512], iv[SM4_BLOCK_SIZE] = {0}, ivbuf[64];
    uint8_t k[64], P[512];
    size_t clen = 0, kclen = 0, s1len = 0, klen = 0, plen = 0, outlen = 0, ivlen = 0;
    char path[512];
    FILE *fp;
    SM4_CBC_CTX cctx;

    if (argc != 5) {
        fprintf(stderr, "usage: %s my_priv.pem pass peer_pub.pem indir\n", argv[0]);
        return 1;
    }
    fp = fopen(argv[1], "rb");
    if (!fp || sm2_private_key_info_decrypt_from_pem(&mine, argv[2], fp) != 1) {
        fprintf(stderr, "load my key fail\n");
        return 1;
    }
    fclose(fp);
    fp = fopen(argv[3], "rb");
    if (!fp || sm2_public_key_info_from_pem(&peer, fp) != 1) {
        fprintf(stderr, "load peer key fail\n");
        return 1;
    }
    fclose(fp);

    sprintf(path, "%s/C.bin", argv[4]);
    if (!read_file(path, C, sizeof(C), &clen)) {
        fprintf(stderr, "read C fail\n");
        return 1;
    }
    sprintf(path, "%s/KC.bin", argv[4]);
    if (!read_file(path, KC, sizeof(KC), &kclen)) {
        fprintf(stderr, "read KC fail\n");
        return 1;
    }
    sprintf(path, "%s/S1.bin", argv[4]);
    if (!read_file(path, S1, sizeof(S1), &s1len)) {
        fprintf(stderr, "read S1 fail\n");
        return 1;
    }
    sprintf(path, "%s/iv.bin", argv[4]);
    if (read_file(path, ivbuf, sizeof(ivbuf), &ivlen) && ivlen == SM4_BLOCK_SIZE)
        memcpy(iv, ivbuf, SM4_BLOCK_SIZE);
    else
        printf("note: iv.bin absent, use zero IV\n");

    sm2_verify_init(&vc, &peer, id, strlen(id));
    sm2_verify_update(&vc, C, clen);
    printf("Sm2Very(PKb, S1): %s\n", sm2_verify_finish(&vc, S1, s1len) == 1 ? "PASS" : "FAIL");

    if (sm2_decrypt(&mine, KC, kclen, k, &klen) != 1) {
        fprintf(stderr, "Sm2Dec fail\n");
        return 1;
    }
    printf("k len = %zu\n", klen);

    sm4_cbc_decrypt_init(&cctx, k, iv);
    sm4_cbc_decrypt_update(&cctx, C, clen, P, &outlen);
    plen = outlen;
    sm4_cbc_decrypt_finish(&cctx, P + plen, &outlen);
    plen += outlen;
    P[plen] = 0;
    printf("P = %s\n", P);
    return 0;
}
