int ssl_prepare_clienthello_tlsext(SSL *s)
{
    s->s3->alpn_sent = 0;
    return 1;
}
