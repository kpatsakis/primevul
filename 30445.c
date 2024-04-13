static int rsa_sig_print(BIO *bp, const X509_ALGOR *sigalg,
                         const ASN1_STRING *sig, int indent, ASN1_PCTX *pctx)
{
    if (OBJ_obj2nid(sigalg->algorithm) == NID_rsassaPss) {
        int rv;
        RSA_PSS_PARAMS *pss;
        X509_ALGOR *maskHash;
        pss = rsa_pss_decode(sigalg, &maskHash);
        rv = rsa_pss_param_print(bp, pss, maskHash, indent);
        if (pss)
            RSA_PSS_PARAMS_free(pss);
        if (maskHash)
            X509_ALGOR_free(maskHash);
        if (!rv)
            return 0;
    } else if (!sig && BIO_puts(bp, "\n") <= 0)
        return 0;
    if (sig)
        return X509_signature_dump(bp, sig, indent);
    return 1;
}
