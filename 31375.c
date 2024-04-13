int ssl_prepare_clienthello_tlsext(SSL *s)
{

# ifdef TLSEXT_TYPE_opaque_prf_input
    {
        int r = 1;

        if (s->ctx->tlsext_opaque_prf_input_callback != 0) {
            r = s->ctx->tlsext_opaque_prf_input_callback(s, NULL, 0,
                                                         s->
                                                         ctx->tlsext_opaque_prf_input_callback_arg);
            if (!r)
                return -1;
        }

        if (s->tlsext_opaque_prf_input != NULL) {
            if (s->s3->client_opaque_prf_input != NULL) {
                /* shouldn't really happen */
                OPENSSL_free(s->s3->client_opaque_prf_input);
            }

            if (s->tlsext_opaque_prf_input_len == 0) {
                /* dummy byte just to get non-NULL */
                s->s3->client_opaque_prf_input = OPENSSL_malloc(1);
            } else {
                s->s3->client_opaque_prf_input =
                    BUF_memdup(s->tlsext_opaque_prf_input,
                               s->tlsext_opaque_prf_input_len);
            }
            if (s->s3->client_opaque_prf_input == NULL) {
                SSLerr(SSL_F_SSL_PREPARE_CLIENTHELLO_TLSEXT,
                       ERR_R_MALLOC_FAILURE);
                return -1;
            }
            s->s3->client_opaque_prf_input_len =
                s->tlsext_opaque_prf_input_len;
        }

        if (r == 2)
            /*
             * at callback's request, insist on receiving an appropriate
             * server opaque PRF input
             */
            s->s3->server_opaque_prf_input_len =
                s->tlsext_opaque_prf_input_len;
    }
# endif

    return 1;
}
