int SSL_get_sigalgs(SSL *s, int idx,
                    int *psign, int *phash, int *psignhash,
                    unsigned char *rsig, unsigned char *rhash)
{
    const unsigned char *psig = s->cert->peer_sigalgs;
    if (psig == NULL)
        return 0;
    if (idx >= 0) {
        idx <<= 1;
        if (idx >= (int)s->cert->peer_sigalgslen)
            return 0;
        psig += idx;
        if (rhash)
            *rhash = psig[0];
        if (rsig)
            *rsig = psig[1];
        tls1_lookup_sigalg(phash, psign, psignhash, psig);
    }
    return s->cert->peer_sigalgslen / 2;
}
