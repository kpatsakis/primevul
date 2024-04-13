int tls1_set_server_sigalgs(SSL *s)
{
    int al;
    size_t i;

    /* Clear any shared signature algorithms */
    OPENSSL_free(s->cert->shared_sigalgs);
    s->cert->shared_sigalgs = NULL;
    s->cert->shared_sigalgslen = 0;
    /* Clear certificate digests and validity flags */
    for (i = 0; i < SSL_PKEY_NUM; i++) {
        s->s3->tmp.md[i] = NULL;
        s->s3->tmp.valid_flags[i] = 0;
    }

    /* If sigalgs received process it. */
    if (s->s3->tmp.peer_sigalgs) {
        if (!tls1_process_sigalgs(s)) {
            SSLerr(SSL_F_TLS1_SET_SERVER_SIGALGS, ERR_R_MALLOC_FAILURE);
            al = SSL_AD_INTERNAL_ERROR;
            goto err;
        }
        /* Fatal error is no shared signature algorithms */
        if (!s->cert->shared_sigalgs) {
            SSLerr(SSL_F_TLS1_SET_SERVER_SIGALGS,
                   SSL_R_NO_SHARED_SIGNATURE_ALGORITHMS);
            al = SSL_AD_ILLEGAL_PARAMETER;
            goto err;
        }
    } else {
        ssl_set_default_md(s);
    }
    return 1;
 err:
    ssl3_send_alert(s, SSL3_AL_FATAL, al);
    return 0;
}
