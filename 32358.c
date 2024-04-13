int ssl_prepare_clienthello_tlsext(SSL *s)
{
# ifndef OPENSSL_NO_EC
    /*
     * If we are client and using an elliptic curve cryptography cipher
     * suite, send the point formats and elliptic curves we support.
     */
    int using_ecc = 0;
    int i;
    unsigned char *j;
    unsigned long alg_k, alg_a;
    STACK_OF(SSL_CIPHER) *cipher_stack = SSL_get_ciphers(s);

    for (i = 0; i < sk_SSL_CIPHER_num(cipher_stack); i++) {
        SSL_CIPHER *c = sk_SSL_CIPHER_value(cipher_stack, i);

        alg_k = c->algorithm_mkey;
        alg_a = c->algorithm_auth;
        if ((alg_k & (SSL_kEECDH | SSL_kECDHr | SSL_kECDHe)
             || (alg_a & SSL_aECDSA))) {
            using_ecc = 1;
            break;
        }
    }
    using_ecc = using_ecc && (s->version >= TLS1_VERSION);
    if (using_ecc) {
        if (s->tlsext_ecpointformatlist != NULL)
            OPENSSL_free(s->tlsext_ecpointformatlist);
        if ((s->tlsext_ecpointformatlist = OPENSSL_malloc(3)) == NULL) {
            SSLerr(SSL_F_SSL_PREPARE_CLIENTHELLO_TLSEXT,
                   ERR_R_MALLOC_FAILURE);
            return -1;
        }
        s->tlsext_ecpointformatlist_length = 3;
        s->tlsext_ecpointformatlist[0] = TLSEXT_ECPOINTFORMAT_uncompressed;
        s->tlsext_ecpointformatlist[1] =
            TLSEXT_ECPOINTFORMAT_ansiX962_compressed_prime;
        s->tlsext_ecpointformatlist[2] =
            TLSEXT_ECPOINTFORMAT_ansiX962_compressed_char2;

        /* we support all named elliptic curves in RFC 4492 */
        if (s->tlsext_ellipticcurvelist != NULL)
            OPENSSL_free(s->tlsext_ellipticcurvelist);
        s->tlsext_ellipticcurvelist_length =
            sizeof(pref_list) / sizeof(pref_list[0]) * 2;
        if ((s->tlsext_ellipticcurvelist =
             OPENSSL_malloc(s->tlsext_ellipticcurvelist_length)) == NULL) {
            s->tlsext_ellipticcurvelist_length = 0;
            SSLerr(SSL_F_SSL_PREPARE_CLIENTHELLO_TLSEXT,
                   ERR_R_MALLOC_FAILURE);
            return -1;
        }
        for (i = 0, j = s->tlsext_ellipticcurvelist; (unsigned int)i <
             sizeof(pref_list) / sizeof(pref_list[0]); i++) {
            int id = tls1_ec_nid2curve_id(pref_list[i]);
            s2n(id, j);
        }
    }
# endif                         /* OPENSSL_NO_EC */

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
