static int tls12_get_pkey_idx(unsigned char sig_alg)
{
    switch (sig_alg) {
#ifndef OPENSSL_NO_RSA
    case TLSEXT_signature_rsa:
        return SSL_PKEY_RSA_SIGN;
#endif
#ifndef OPENSSL_NO_DSA
    case TLSEXT_signature_dsa:
        return SSL_PKEY_DSA_SIGN;
#endif
#ifndef OPENSSL_NO_EC
    case TLSEXT_signature_ecdsa:
        return SSL_PKEY_ECC;
#endif
#ifndef OPENSSL_NO_GOST
    case TLSEXT_signature_gostr34102001:
        return SSL_PKEY_GOST01;

    case TLSEXT_signature_gostr34102012_256:
        return SSL_PKEY_GOST12_256;

    case TLSEXT_signature_gostr34102012_512:
        return SSL_PKEY_GOST12_512;
#endif
    }
    return -1;
}
