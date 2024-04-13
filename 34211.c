static int add_session(SSL *ssl, SSL_SESSION *session)
{
    simple_ssl_session *sess;
    unsigned char *p;

    sess = OPENSSL_malloc(sizeof(simple_ssl_session));
    if (!sess) {
        BIO_printf(bio_err, "Out of memory adding session to external cache\n");
        return 0;
    }

    SSL_SESSION_get_id(session, &sess->idlen);
    sess->derlen = i2d_SSL_SESSION(session, NULL);

    sess->id = BUF_memdup(SSL_SESSION_get_id(session, NULL), sess->idlen);

    sess->der = OPENSSL_malloc(sess->derlen);
    if (!sess->id || !sess->der) {
        BIO_printf(bio_err, "Out of memory adding session to external cache\n");

        if (sess->id)
            OPENSSL_free(sess->id);
        if (sess->der)
            OPENSSL_free(sess->der);
        OPENSSL_free(sess);
        return 0;
    }
    p = sess->der;
    i2d_SSL_SESSION(session, &p);

    sess->next = first;
    first = sess;
    BIO_printf(bio_err, "New session added to external cache\n");
    return 0;
}
