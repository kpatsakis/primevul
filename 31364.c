int SSL_get_shared_sigalgs(SSL *s, int idx,
                           int *psign, int *phash, int *psignhash,
                           unsigned char *rsig, unsigned char *rhash)
{
    TLS_SIGALGS *shsigalgs = s->cert->shared_sigalgs;
    if (!shsigalgs || idx >= (int)s->cert->shared_sigalgslen)
        return 0;
    shsigalgs += idx;
    if (phash)
        *phash = shsigalgs->hash_nid;
    if (psign)
        *psign = shsigalgs->sign_nid;
    if (psignhash)
        *psignhash = shsigalgs->signandhash_nid;
    if (rsig)
        *rsig = shsigalgs->rsign;
    if (rhash)
        *rhash = shsigalgs->rhash;
    return s->cert->shared_sigalgslen;
}
