int tls12_get_sigandhash(unsigned char *p, const EVP_PKEY *pk,
                         const EVP_MD *md)
{
    int sig_id, md_id;
    if (!md)
        return 0;
    md_id = tls12_find_id(EVP_MD_type(md), tls12_md,
                          sizeof(tls12_md) / sizeof(tls12_lookup));
    if (md_id == -1)
        return 0;
    sig_id = tls12_get_sigid(pk);
    if (sig_id == -1)
        return 0;
    p[0] = (unsigned char)md_id;
    p[1] = (unsigned char)sig_id;
    return 1;
}
