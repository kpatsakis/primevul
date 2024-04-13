static int do_rsa_print(BIO *bp, const RSA *x, int off, int priv)
{
    char *str;
    const char *s;
    unsigned char *m = NULL;
    int ret = 0, mod_len = 0;
    size_t buf_len = 0;

    update_buflen(x->n, &buf_len);
    update_buflen(x->e, &buf_len);

    if (priv) {
        update_buflen(x->d, &buf_len);
        update_buflen(x->p, &buf_len);
        update_buflen(x->q, &buf_len);
        update_buflen(x->dmp1, &buf_len);
        update_buflen(x->dmq1, &buf_len);
        update_buflen(x->iqmp, &buf_len);
    }

    m = (unsigned char *)OPENSSL_malloc(buf_len + 10);
    if (m == NULL) {
        RSAerr(RSA_F_DO_RSA_PRINT, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    if (x->n != NULL)
        mod_len = BN_num_bits(x->n);

    if (!BIO_indent(bp, off, 128))
        goto err;

    if (priv && x->d) {
        if (BIO_printf(bp, "Private-Key: (%d bit)\n", mod_len)
            <= 0)
            goto err;
        str = "modulus:";
        s = "publicExponent:";
    } else {
        if (BIO_printf(bp, "Public-Key: (%d bit)\n", mod_len)
            <= 0)
            goto err;
        str = "Modulus:";
        s = "Exponent:";
    }
    if (!ASN1_bn_print(bp, str, x->n, m, off))
        goto err;
    if (!ASN1_bn_print(bp, s, x->e, m, off))
        goto err;
    if (priv) {
        if (!ASN1_bn_print(bp, "privateExponent:", x->d, m, off))
            goto err;
        if (!ASN1_bn_print(bp, "prime1:", x->p, m, off))
            goto err;
        if (!ASN1_bn_print(bp, "prime2:", x->q, m, off))
            goto err;
        if (!ASN1_bn_print(bp, "exponent1:", x->dmp1, m, off))
            goto err;
        if (!ASN1_bn_print(bp, "exponent2:", x->dmq1, m, off))
            goto err;
        if (!ASN1_bn_print(bp, "coefficient:", x->iqmp, m, off))
            goto err;
    }
    ret = 1;
 err:
    if (m != NULL)
        OPENSSL_free(m);
    return (ret);
}
