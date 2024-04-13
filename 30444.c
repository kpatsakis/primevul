static int rsa_pub_print(BIO *bp, const EVP_PKEY *pkey, int indent,
                         ASN1_PCTX *ctx)
{
    return do_rsa_print(bp, pkey->pkey.rsa, indent, 0);
}
