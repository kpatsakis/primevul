int do_dtls1_write(SSL *s, int type, const unsigned char *buf,
                   unsigned int len, int create_empty_fragment)
{
    unsigned char *p, *pseq;
    int i, mac_size, clear = 0;
    int prefix_len = 0;
    int eivlen;
    SSL3_RECORD wr;
    SSL3_BUFFER *wb;
    SSL_SESSION *sess;

    wb = &s->rlayer.wbuf[0];

    /*
     * first check if there is a SSL3_BUFFER still being written out.  This
     * will happen with non blocking IO
     */
    if (SSL3_BUFFER_get_left(wb) != 0) {
        OPENSSL_assert(0);      /* XDTLS: want to see if we ever get here */
        return (ssl3_write_pending(s, type, buf, len));
    }

    /* If we have an alert to send, lets send it */
    if (s->s3->alert_dispatch) {
        i = s->method->ssl_dispatch_alert(s);
        if (i <= 0)
            return (i);
        /* if it went, fall through and send more stuff */
    }

    if (len == 0 && !create_empty_fragment)
        return 0;

    sess = s->session;

    if ((sess == NULL) ||
        (s->enc_write_ctx == NULL) || (EVP_MD_CTX_md(s->write_hash) == NULL))
        clear = 1;

    if (clear)
        mac_size = 0;
    else {
        mac_size = EVP_MD_CTX_size(s->write_hash);
        if (mac_size < 0)
            goto err;
    }

    p = SSL3_BUFFER_get_buf(wb) + prefix_len;

    /* write the header */

    *(p++) = type & 0xff;
    SSL3_RECORD_set_type(&wr, type);
    /*
     * Special case: for hello verify request, client version 1.0 and we
     * haven't decided which version to use yet send back using version 1.0
     * header: otherwise some clients will ignore it.
     */
    if (s->method->version == DTLS_ANY_VERSION &&
        s->max_proto_version != DTLS1_BAD_VER) {
        *(p++) = DTLS1_VERSION >> 8;
        *(p++) = DTLS1_VERSION & 0xff;
    } else {
        *(p++) = s->version >> 8;
        *(p++) = s->version & 0xff;
    }

    /* field where we are to write out packet epoch, seq num and len */
    pseq = p;
    p += 10;

    /* Explicit IV length, block ciphers appropriate version flag */
    if (s->enc_write_ctx) {
        int mode = EVP_CIPHER_CTX_mode(s->enc_write_ctx);
        if (mode == EVP_CIPH_CBC_MODE) {
            eivlen = EVP_CIPHER_CTX_iv_length(s->enc_write_ctx);
            if (eivlen <= 1)
                eivlen = 0;
        }
        /* Need explicit part of IV for GCM mode */
        else if (mode == EVP_CIPH_GCM_MODE)
            eivlen = EVP_GCM_TLS_EXPLICIT_IV_LEN;
        else if (mode == EVP_CIPH_CCM_MODE)
            eivlen = EVP_CCM_TLS_EXPLICIT_IV_LEN;
        else
            eivlen = 0;
    } else
        eivlen = 0;

    /* lets setup the record stuff. */
    SSL3_RECORD_set_data(&wr, p + eivlen); /* make room for IV in case of CBC */
    SSL3_RECORD_set_length(&wr, (int)len);
    SSL3_RECORD_set_input(&wr, (unsigned char *)buf);

    /*
     * we now 'read' from wr.input, wr.length bytes into wr.data
     */

    /* first we compress */
    if (s->compress != NULL) {
        if (!ssl3_do_compress(s, &wr)) {
            SSLerr(SSL_F_DO_DTLS1_WRITE, SSL_R_COMPRESSION_FAILURE);
            goto err;
        }
    } else {
        memcpy(SSL3_RECORD_get_data(&wr), SSL3_RECORD_get_input(&wr),
               SSL3_RECORD_get_length(&wr));
        SSL3_RECORD_reset_input(&wr);
    }

    /*
     * we should still have the output to wr.data and the input from
     * wr.input.  Length should be wr.length. wr.data still points in the
     * wb->buf
     */

    if (mac_size != 0) {
        if (s->method->ssl3_enc->mac(s, &wr,
                                     &(p[SSL3_RECORD_get_length(&wr) + eivlen]),
                                     1) < 0)
            goto err;
        SSL3_RECORD_add_length(&wr, mac_size);
    }

    /* this is true regardless of mac size */
    SSL3_RECORD_set_data(&wr, p);
    SSL3_RECORD_reset_input(&wr);

    if (eivlen)
        SSL3_RECORD_add_length(&wr, eivlen);

    if (s->method->ssl3_enc->enc(s, &wr, 1, 1) < 1)
        goto err;

    /* record length after mac and block padding */
    /*
     * if (type == SSL3_RT_APPLICATION_DATA || (type == SSL3_RT_ALERT && !
     * SSL_in_init(s)))
     */

    /* there's only one epoch between handshake and app data */

    s2n(s->rlayer.d->w_epoch, pseq);

    /* XDTLS: ?? */
    /*
     * else s2n(s->d1->handshake_epoch, pseq);
     */

    memcpy(pseq, &(s->rlayer.write_sequence[2]), 6);
    pseq += 6;
    s2n(SSL3_RECORD_get_length(&wr), pseq);

    if (s->msg_callback)
        s->msg_callback(1, 0, SSL3_RT_HEADER, pseq - DTLS1_RT_HEADER_LENGTH,
                        DTLS1_RT_HEADER_LENGTH, s, s->msg_callback_arg);

    /*
     * we should now have wr.data pointing to the encrypted data, which is
     * wr->length long
     */
    SSL3_RECORD_set_type(&wr, type); /* not needed but helps for debugging */
    SSL3_RECORD_add_length(&wr, DTLS1_RT_HEADER_LENGTH);

    ssl3_record_sequence_update(&(s->rlayer.write_sequence[0]));

    if (create_empty_fragment) {
        /*
         * we are in a recursive call; just return the length, don't write
         * out anything here
         */
        return wr.length;
    }

    /* now let's set up wb */
    SSL3_BUFFER_set_left(wb, prefix_len + SSL3_RECORD_get_length(&wr));
    SSL3_BUFFER_set_offset(wb, 0);

    /*
     * memorize arguments so that ssl3_write_pending can detect bad write
     * retries later
     */
    s->rlayer.wpend_tot = len;
    s->rlayer.wpend_buf = buf;
    s->rlayer.wpend_type = type;
    s->rlayer.wpend_ret = len;

    /* we now just need to write the buffer */
    return ssl3_write_pending(s, type, buf, len);
 err:
    return -1;
}
