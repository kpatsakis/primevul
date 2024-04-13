int tls12_get_req_sig_algs(SSL *s, unsigned char *p)
{
    size_t slen = sizeof(tls12_sigalgs);
    if (p)
        memcpy(p, tls12_sigalgs, slen);
    return (int)slen;
}
