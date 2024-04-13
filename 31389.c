static int tls1_alpn_handle_client_hello(SSL *s, const unsigned char *data,
                                         unsigned data_len, int *al)
{
    unsigned i;
    unsigned proto_len;
    const unsigned char *selected;
    unsigned char selected_len;
    int r;

    if (s->ctx->alpn_select_cb == NULL)
        return 0;

    if (data_len < 2)
        goto parse_error;

    /*
     * data should contain a uint16 length followed by a series of 8-bit,
     * length-prefixed strings.
     */
    i = ((unsigned)data[0]) << 8 | ((unsigned)data[1]);
    data_len -= 2;
    data += 2;
    if (data_len != i)
        goto parse_error;

    if (data_len < 2)
        goto parse_error;

    for (i = 0; i < data_len;) {
        proto_len = data[i];
        i++;

        if (proto_len == 0)
            goto parse_error;

        if (i + proto_len < i || i + proto_len > data_len)
            goto parse_error;

        i += proto_len;
    }

    r = s->ctx->alpn_select_cb(s, &selected, &selected_len, data, data_len,
                               s->ctx->alpn_select_cb_arg);
    if (r == SSL_TLSEXT_ERR_OK) {
        if (s->s3->alpn_selected)
            OPENSSL_free(s->s3->alpn_selected);
        s->s3->alpn_selected = OPENSSL_malloc(selected_len);
        if (!s->s3->alpn_selected) {
            *al = SSL_AD_INTERNAL_ERROR;
            return -1;
        }
        memcpy(s->s3->alpn_selected, selected, selected_len);
        s->s3->alpn_selected_len = selected_len;
    }
    return 0;

 parse_error:
    *al = SSL_AD_DECODE_ERROR;
    return -1;
}
