static int rsa_pub_decode(EVP_PKEY *pkey, X509_PUBKEY *pubkey)
{
    const unsigned char *p;
    int pklen;
    RSA *rsa = NULL;
    if (!X509_PUBKEY_get0_param(NULL, &p, &pklen, NULL, pubkey))
        return 0;
    if (!(rsa = d2i_RSAPublicKey(NULL, &p, pklen))) {
        RSAerr(RSA_F_RSA_PUB_DECODE, ERR_R_RSA_LIB);
        return 0;
    }
    EVP_PKEY_assign_RSA(pkey, rsa);
    return 1;
}
