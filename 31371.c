int ssl_check_serverhello_tlsext(SSL *s)
{
    int ret = SSL_TLSEXT_ERR_NOACK;
    int al = SSL_AD_UNRECOGNIZED_NAME;

# ifndef OPENSSL_NO_EC
    /*
     * If we are client and using an elliptic curve cryptography cipher
     * suite, then if server returns an EC point formats lists extension it
     * must contain uncompressed.
     */
    unsigned long alg_k = s->s3->tmp.new_cipher->algorithm_mkey;
    unsigned long alg_a = s->s3->tmp.new_cipher->algorithm_auth;
    if ((s->tlsext_ecpointformatlist != NULL)
        && (s->tlsext_ecpointformatlist_length > 0)
        && (s->session->tlsext_ecpointformatlist != NULL)
        && (s->session->tlsext_ecpointformatlist_length > 0)
        && ((alg_k & (SSL_kEECDH | SSL_kECDHr | SSL_kECDHe))
            || (alg_a & SSL_aECDSA))) {
        /* we are using an ECC cipher */
        size_t i;
        unsigned char *list;
        int found_uncompressed = 0;
        list = s->session->tlsext_ecpointformatlist;
        for (i = 0; i < s->session->tlsext_ecpointformatlist_length; i++) {
            if (*(list++) == TLSEXT_ECPOINTFORMAT_uncompressed) {
                found_uncompressed = 1;
                break;
            }
        }
        if (!found_uncompressed) {
            SSLerr(SSL_F_SSL_CHECK_SERVERHELLO_TLSEXT,
                   SSL_R_TLS_INVALID_ECPOINTFORMAT_LIST);
            return -1;
        }
    }
    ret = SSL_TLSEXT_ERR_OK;
# endif                         /* OPENSSL_NO_EC */

    if (s->ctx != NULL && s->ctx->tlsext_servername_callback != 0)
        ret =
            s->ctx->tlsext_servername_callback(s, &al,
                                               s->ctx->tlsext_servername_arg);
    else if (s->initial_ctx != NULL
             && s->initial_ctx->tlsext_servername_callback != 0)
        ret =
            s->initial_ctx->tlsext_servername_callback(s, &al,
                                                       s->
                                                       initial_ctx->tlsext_servername_arg);

# ifdef TLSEXT_TYPE_opaque_prf_input
    if (s->s3->server_opaque_prf_input_len > 0) {
        /*
         * This case may indicate that we, as a client, want to insist on
         * using opaque PRF inputs. So first verify that we really have a
         * value from the server too.
         */

        if (s->s3->server_opaque_prf_input == NULL) {
            ret = SSL_TLSEXT_ERR_ALERT_FATAL;
            al = SSL_AD_HANDSHAKE_FAILURE;
        }

        /*
         * Anytime the server *has* sent an opaque PRF input, we need to
         * check that we have a client opaque PRF input of the same size.
         */
        if (s->s3->client_opaque_prf_input == NULL ||
            s->s3->client_opaque_prf_input_len !=
            s->s3->server_opaque_prf_input_len) {
            ret = SSL_TLSEXT_ERR_ALERT_FATAL;
            al = SSL_AD_ILLEGAL_PARAMETER;
        }
    }
# endif

    /*
     * If we've requested certificate status and we wont get one tell the
     * callback
     */
    if ((s->tlsext_status_type != -1) && !(s->tlsext_status_expected)
        && s->ctx && s->ctx->tlsext_status_cb) {
        int r;
        /*
         * Set resp to NULL, resplen to -1 so callback knows there is no
         * response.
         */
        if (s->tlsext_ocsp_resp) {
            OPENSSL_free(s->tlsext_ocsp_resp);
            s->tlsext_ocsp_resp = NULL;
        }
        s->tlsext_ocsp_resplen = -1;
        r = s->ctx->tlsext_status_cb(s, s->ctx->tlsext_status_arg);
        if (r == 0) {
            al = SSL_AD_BAD_CERTIFICATE_STATUS_RESPONSE;
            ret = SSL_TLSEXT_ERR_ALERT_FATAL;
        }
        if (r < 0) {
            al = SSL_AD_INTERNAL_ERROR;
            ret = SSL_TLSEXT_ERR_ALERT_FATAL;
        }
    }

    switch (ret) {
    case SSL_TLSEXT_ERR_ALERT_FATAL:
        ssl3_send_alert(s, SSL3_AL_FATAL, al);
        return -1;

    case SSL_TLSEXT_ERR_ALERT_WARNING:
        ssl3_send_alert(s, SSL3_AL_WARNING, al);
        return 1;

    case SSL_TLSEXT_ERR_NOACK:
        s->servername_done = 0;
    default:
        return 1;
    }
}
