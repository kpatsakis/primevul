static int tls1_set_shared_sigalgs(SSL *s)
{
    const unsigned char *pref, *allow, *conf;
    size_t preflen, allowlen, conflen;
    size_t nmatch;
    TLS_SIGALGS *salgs = NULL;
    CERT *c = s->cert;
    unsigned int is_suiteb = tls1_suiteb(s);

    OPENSSL_free(c->shared_sigalgs);
    c->shared_sigalgs = NULL;
    c->shared_sigalgslen = 0;
    /* If client use client signature algorithms if not NULL */
    if (!s->server && c->client_sigalgs && !is_suiteb) {
        conf = c->client_sigalgs;
        conflen = c->client_sigalgslen;
    } else if (c->conf_sigalgs && !is_suiteb) {
        conf = c->conf_sigalgs;
        conflen = c->conf_sigalgslen;
    } else
        conflen = tls12_get_psigalgs(s, &conf);
    if (s->options & SSL_OP_CIPHER_SERVER_PREFERENCE || is_suiteb) {
        pref = conf;
        preflen = conflen;
        allow = s->s3->tmp.peer_sigalgs;
        allowlen = s->s3->tmp.peer_sigalgslen;
    } else {
        allow = conf;
        allowlen = conflen;
        pref = s->s3->tmp.peer_sigalgs;
        preflen = s->s3->tmp.peer_sigalgslen;
    }
    nmatch = tls12_shared_sigalgs(s, NULL, pref, preflen, allow, allowlen);
    if (nmatch) {
        salgs = OPENSSL_malloc(nmatch * sizeof(TLS_SIGALGS));
        if (salgs == NULL)
            return 0;
        nmatch = tls12_shared_sigalgs(s, salgs, pref, preflen, allow, allowlen);
    } else {
        salgs = NULL;
    }
    c->shared_sigalgs = salgs;
    c->shared_sigalgslen = nmatch;
    return 1;
}
