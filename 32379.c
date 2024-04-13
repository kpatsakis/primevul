int ssl_check_clienthello_tlsext_late(SSL *s, int *al)
{
    s->tlsext_status_expected = 0;

    /*
     * If status request then ask callback what to do. Note: this must be
     * called after servername callbacks in case the certificate has changed,
     * and must be called after the cipher has been chosen because this may
     * influence which certificate is sent
     */
    if ((s->tlsext_status_type != -1) && s->ctx && s->ctx->tlsext_status_cb) {
        int ret;
        CERT_PKEY *certpkey;
        certpkey = ssl_get_server_send_pkey(s);
        /* If no certificate can't return certificate status */
        if (certpkey != NULL) {
            /*
             * Set current certificate to one we will use so SSL_get_certificate
             * et al can pick it up.
             */
            s->cert->key = certpkey;
            ret = s->ctx->tlsext_status_cb(s, s->ctx->tlsext_status_arg);
            switch (ret) {
                /* We don't want to send a status request response */
            case SSL_TLSEXT_ERR_NOACK:
                s->tlsext_status_expected = 0;
                break;
                /* status request response should be sent */
            case SSL_TLSEXT_ERR_OK:
                if (s->tlsext_ocsp_resp)
                    s->tlsext_status_expected = 1;
                break;
                /* something bad happened */
            case SSL_TLSEXT_ERR_ALERT_FATAL:
            default:
                *al = SSL_AD_INTERNAL_ERROR;
                return 0;
            }
        }
    }

    if (!tls1_alpn_handle_client_hello_late(s, al)) {
        return 0;
    }

    return 1;
}
