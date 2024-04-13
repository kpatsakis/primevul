static int PKCS7_bio_add_digest(BIO **pbio, X509_ALGOR *alg)
{
    BIO *btmp;
    const EVP_MD *md;
    if ((btmp = BIO_new(BIO_f_md())) == NULL) {
        PKCS7err(PKCS7_F_PKCS7_BIO_ADD_DIGEST, ERR_R_BIO_LIB);
        goto err;
    }

    md = EVP_get_digestbyobj(alg->algorithm);
    if (md == NULL) {
        PKCS7err(PKCS7_F_PKCS7_BIO_ADD_DIGEST, PKCS7_R_UNKNOWN_DIGEST_TYPE);
        goto err;
    }

    BIO_set_md(btmp, md);
    if (*pbio == NULL)
        *pbio = btmp;
    else if (!BIO_push(*pbio, btmp)) {
        PKCS7err(PKCS7_F_PKCS7_BIO_ADD_DIGEST, ERR_R_BIO_LIB);
        goto err;
    }
    btmp = NULL;

    return 1;

 err:
    BIO_free(btmp);
    return 0;

}
