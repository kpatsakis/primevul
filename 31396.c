void tls1_clear(SSL *s)
{
    ssl3_clear(s);
    s->version = s->method->version;
}
