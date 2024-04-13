int tls1_process_sigalgs(SSL *s, const unsigned char *data, int dsize)
{
    int i, idx;
    const EVP_MD *md;
    CERT *c = s->cert;
    /* Extension ignored for TLS versions below 1.2 */
    if (TLS1_get_version(s) < TLS1_2_VERSION)
        return 1;
    /* Should never happen */
    if (!c)
        return 0;

    c->pkeys[SSL_PKEY_DSA_SIGN].digest = NULL;
    c->pkeys[SSL_PKEY_RSA_SIGN].digest = NULL;
    c->pkeys[SSL_PKEY_RSA_ENC].digest = NULL;
    c->pkeys[SSL_PKEY_ECC].digest = NULL;

    for (i = 0; i < dsize; i += 2) {
        unsigned char hash_alg = data[i], sig_alg = data[i + 1];

        switch (sig_alg) {
# ifndef OPENSSL_NO_RSA
        case TLSEXT_signature_rsa:
            idx = SSL_PKEY_RSA_SIGN;
            break;
# endif
# ifndef OPENSSL_NO_DSA
        case TLSEXT_signature_dsa:
            idx = SSL_PKEY_DSA_SIGN;
            break;
# endif
# ifndef OPENSSL_NO_ECDSA
        case TLSEXT_signature_ecdsa:
            idx = SSL_PKEY_ECC;
            break;
# endif
        default:
            continue;
        }

        if (c->pkeys[idx].digest == NULL) {
            md = tls12_get_hash(hash_alg);
            if (md) {
                c->pkeys[idx].digest = md;
                if (idx == SSL_PKEY_RSA_SIGN)
                    c->pkeys[SSL_PKEY_RSA_ENC].digest = md;
            }
        }

    }

    /*
     * Set any remaining keys to default values. NOTE: if alg is not
     * supported it stays as NULL.
     */
# ifndef OPENSSL_NO_DSA
    if (!c->pkeys[SSL_PKEY_DSA_SIGN].digest)
        c->pkeys[SSL_PKEY_DSA_SIGN].digest = EVP_sha1();
# endif
# ifndef OPENSSL_NO_RSA
    if (!c->pkeys[SSL_PKEY_RSA_SIGN].digest) {
        c->pkeys[SSL_PKEY_RSA_SIGN].digest = EVP_sha1();
        c->pkeys[SSL_PKEY_RSA_ENC].digest = EVP_sha1();
    }
# endif
# ifndef OPENSSL_NO_ECDSA
    if (!c->pkeys[SSL_PKEY_ECC].digest)
        c->pkeys[SSL_PKEY_ECC].digest = EVP_sha1();
# endif
    return 1;
}
