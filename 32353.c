int ssl_check_clienthello_tlsext_early(SSL *s)
{
    int ret = SSL_TLSEXT_ERR_NOACK;
    int al = SSL_AD_UNRECOGNIZED_NAME;

# ifndef OPENSSL_NO_EC
    /*
     * The handling of the ECPointFormats extension is done elsewhere, namely
     * in ssl3_choose_cipher in s3_lib.c.
     */
    /*
     * The handling of the EllipticCurves extension is done elsewhere, namely
     * in ssl3_choose_cipher in s3_lib.c.
     */
# endif

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
    {
        /*
         * This sort of belongs into ssl_prepare_serverhello_tlsext(), but we
         * might be sending an alert in response to the client hello, so this
         * has to happen here in ssl_check_clienthello_tlsext_early().
         */

        int r = 1;

        if (s->ctx->tlsext_opaque_prf_input_callback != 0) {
            r = s->ctx->tlsext_opaque_prf_input_callback(s, NULL, 0,
                                                         s->
                                                         ctx->tlsext_opaque_prf_input_callback_arg);
            if (!r) {
                ret = SSL_TLSEXT_ERR_ALERT_FATAL;
                al = SSL_AD_INTERNAL_ERROR;
                goto err;
            }
        }

        if (s->s3->server_opaque_prf_input != NULL) {
            /* shouldn't really happen */
            OPENSSL_free(s->s3->server_opaque_prf_input);
        }
        s->s3->server_opaque_prf_input = NULL;

        if (s->tlsext_opaque_prf_input != NULL) {
            if (s->s3->client_opaque_prf_input != NULL &&
                s->s3->client_opaque_prf_input_len ==
                s->tlsext_opaque_prf_input_len) {
                /*
                 * can only use this extension if we have a server opaque PRF
                 * input of the same length as the client opaque PRF input!
                 */

                if (s->tlsext_opaque_prf_input_len == 0) {
                    /* dummy byte just to get non-NULL */
                    s->s3->server_opaque_prf_input = OPENSSL_malloc(1);
                } else {
                    s->s3->server_opaque_prf_input =
                        BUF_memdup(s->tlsext_opaque_prf_input,
                                   s->tlsext_opaque_prf_input_len);
                }
                if (s->s3->server_opaque_prf_input == NULL) {
                    ret = SSL_TLSEXT_ERR_ALERT_FATAL;
                    al = SSL_AD_INTERNAL_ERROR;
                    goto err;
                }
                s->s3->server_opaque_prf_input_len =
                    s->tlsext_opaque_prf_input_len;
            }
        }

        if (r == 2 && s->s3->server_opaque_prf_input == NULL) {
            /*
             * The callback wants to enforce use of the extension, but we
             * can't do that with the client opaque PRF input; abort the
             * handshake.
             */
            ret = SSL_TLSEXT_ERR_ALERT_FATAL;
            al = SSL_AD_HANDSHAKE_FAILURE;
        }
    }

 err:
# endif
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
