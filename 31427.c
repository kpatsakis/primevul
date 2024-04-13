int X509_REQ_check_private_key(X509_REQ *x, EVP_PKEY *k)
{
    EVP_PKEY *xk = NULL;
    int ok = 0;

    xk = X509_REQ_get_pubkey(x);
    switch (EVP_PKEY_cmp(xk, k)) {
    case 1:
        ok = 1;
        break;
    case 0:
        X509err(X509_F_X509_REQ_CHECK_PRIVATE_KEY,
                X509_R_KEY_VALUES_MISMATCH);
        break;
    case -1:
        X509err(X509_F_X509_REQ_CHECK_PRIVATE_KEY, X509_R_KEY_TYPE_MISMATCH);
        break;
    case -2:
#ifndef OPENSSL_NO_EC
        if (k->type == EVP_PKEY_EC) {
            X509err(X509_F_X509_REQ_CHECK_PRIVATE_KEY, ERR_R_EC_LIB);
            break;
        }
#endif
#ifndef OPENSSL_NO_DH
        if (k->type == EVP_PKEY_DH) {
            /* No idea */
            X509err(X509_F_X509_REQ_CHECK_PRIVATE_KEY,
                    X509_R_CANT_CHECK_DH_KEY);
            break;
        }
#endif
        X509err(X509_F_X509_REQ_CHECK_PRIVATE_KEY, X509_R_UNKNOWN_KEY_TYPE);
    }

    EVP_PKEY_free(xk);
    return (ok);
}
