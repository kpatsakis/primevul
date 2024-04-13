void tls1_free(SSL *s)
{
#ifndef OPENSSL_NO_TLSEXT
    if (s->tlsext_session_ticket) {
        OPENSSL_free(s->tlsext_session_ticket);
    }
#endif                          /* OPENSSL_NO_TLSEXT */
    ssl3_free(s);
}
