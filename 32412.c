int tls1_save_sigalgs(SSL *s, const unsigned char *data, int dsize)
{
    CERT *c = s->cert;
    /* Extension ignored for inappropriate versions */
    if (!SSL_USE_SIGALGS(s))
        return 1;
    /* Should never happen */
    if (!c)
        return 0;

    OPENSSL_free(s->s3->tmp.peer_sigalgs);
    s->s3->tmp.peer_sigalgs = OPENSSL_malloc(dsize);
    if (s->s3->tmp.peer_sigalgs == NULL)
        return 0;
    s->s3->tmp.peer_sigalgslen = dsize;
    memcpy(s->s3->tmp.peer_sigalgs, data, dsize);
    return 1;
}
