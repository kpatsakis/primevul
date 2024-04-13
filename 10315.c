static int SWTPM_HMAC(unsigned char *md, unsigned int *md_len,
                      const void *key, int key_len,
                      const unsigned char *in, uint32_t in_length,
                      const unsigned char *ivec, uint32_t ivec_length)
{
    int ret = 0;

#if defined OPENSSL_OLD_API
    HMAC_CTX sctx, *ctx = &sctx;

    HMAC_CTX_init(ctx);
#else
    HMAC_CTX *ctx = HMAC_CTX_new();

    if (!ctx)
        return 0;
#endif


    if (!HMAC_Init_ex(ctx, key, key_len, EVP_sha256(), NULL) ||
        !HMAC_Update(ctx, in, in_length))
        goto err;

    if (ivec &&
        !HMAC_Update(ctx, ivec, ivec_length))
        goto err;

    if (!HMAC_Final(ctx, md, md_len))
        goto err;

    ret = 1;

err:
#if defined OPENSSL_OLD_API
    HMAC_CTX_cleanup(ctx);
#else
    HMAC_CTX_free(ctx);
#endif

    return ret;
}