size_t tls12_copy_sigalgs(SSL *s, unsigned char *out,
                          const unsigned char *psig, size_t psiglen)
{
    unsigned char *tmpout = out;
    size_t i;
    for (i = 0; i < psiglen; i += 2, psig += 2) {
        if (tls12_sigalg_allowed(s, SSL_SECOP_SIGALG_SUPPORTED, psig)) {
            *tmpout++ = psig[0];
            *tmpout++ = psig[1];
        }
    }
    return tmpout - out;
}
