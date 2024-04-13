static int have_handshake_fragment(SSL *s, int type, unsigned char *buf,
                                   int len)
{

    if ((type == SSL3_RT_HANDSHAKE)
        && (s->rlayer.d->handshake_fragment_len > 0))
        /* (partially) satisfy request from storage */
    {
        unsigned char *src = s->rlayer.d->handshake_fragment;
        unsigned char *dst = buf;
        unsigned int k, n;

        /* peek == 0 */
        n = 0;
        while ((len > 0) && (s->rlayer.d->handshake_fragment_len > 0)) {
            *dst++ = *src++;
            len--;
            s->rlayer.d->handshake_fragment_len--;
            n++;
        }
        /* move any remaining fragment bytes: */
        for (k = 0; k < s->rlayer.d->handshake_fragment_len; k++)
            s->rlayer.d->handshake_fragment[k] = *src++;
        return n;
    }

    return 0;
}
