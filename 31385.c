static int tls12_get_pkey_idx(unsigned char sig_alg)
{
    switch (sig_alg) {
# ifndef OPENSSL_NO_RSA
    case TLSEXT_signature_rsa:
        return SSL_PKEY_RSA_SIGN;
# endif
# ifndef OPENSSL_NO_DSA
    case TLSEXT_signature_dsa:
        return SSL_PKEY_DSA_SIGN;
# endif
# ifndef OPENSSL_NO_ECDSA
    case TLSEXT_signature_ecdsa:
        return SSL_PKEY_ECC;
# endif
    }
    return -1;
}
