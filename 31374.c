int ssl_parse_serverhello_tlsext(SSL *s, unsigned char **p, unsigned char *d,
                                 int n)
{
    int al = -1;
    if (s->version < SSL3_VERSION)
        return 1;
    if (ssl_scan_serverhello_tlsext(s, p, d, n, &al) <= 0) {
        ssl3_send_alert(s, SSL3_AL_FATAL, al);
        return 0;
    }

    if (ssl_check_serverhello_tlsext(s) <= 0) {
        SSLerr(SSL_F_SSL_PARSE_SERVERHELLO_TLSEXT, SSL_R_SERVERHELLO_TLSEXT);
        return 0;
    }
    return 1;
}
