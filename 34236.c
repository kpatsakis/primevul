static SRP_gN_cache *SRP_gN_new_init(const char *ch)
{
    unsigned char tmp[MAX_LEN];
    int len;

    SRP_gN_cache *newgN =
        (SRP_gN_cache *)OPENSSL_malloc(sizeof(SRP_gN_cache));
    if (newgN == NULL)
        return NULL;

    if ((newgN->b64_bn = BUF_strdup(ch)) == NULL)
        goto err;

    len = t_fromb64(tmp, ch);
    if ((newgN->bn = BN_bin2bn(tmp, len, NULL)))
        return newgN;

    OPENSSL_free(newgN->b64_bn);
 err:
    OPENSSL_free(newgN);
    return NULL;
}
