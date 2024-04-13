static int tls_use_ticket(SSL *s)
{
    if (s->options & SSL_OP_NO_TICKET)
        return 0;
    return ssl_security(s, SSL_SECOP_TICKET, 0, 0, NULL);
}
