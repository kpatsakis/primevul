void tls1_set_cert_validity(SSL *s)
{
    tls1_check_chain(s, NULL, NULL, NULL, SSL_PKEY_RSA_ENC);
    tls1_check_chain(s, NULL, NULL, NULL, SSL_PKEY_RSA_SIGN);
    tls1_check_chain(s, NULL, NULL, NULL, SSL_PKEY_DSA_SIGN);
    tls1_check_chain(s, NULL, NULL, NULL, SSL_PKEY_DH_RSA);
    tls1_check_chain(s, NULL, NULL, NULL, SSL_PKEY_DH_DSA);
    tls1_check_chain(s, NULL, NULL, NULL, SSL_PKEY_ECC);
}
