static ASN1_STRING *rsa_ctx_to_pss(EVP_PKEY_CTX *pkctx)
{
    const EVP_MD *sigmd, *mgf1md;
    RSA_PSS_PARAMS *pss = NULL;
    ASN1_STRING *os = NULL;
    EVP_PKEY *pk = EVP_PKEY_CTX_get0_pkey(pkctx);
    int saltlen, rv = 0;
    if (EVP_PKEY_CTX_get_signature_md(pkctx, &sigmd) <= 0)
        goto err;
    if (EVP_PKEY_CTX_get_rsa_mgf1_md(pkctx, &mgf1md) <= 0)
        goto err;
    if (!EVP_PKEY_CTX_get_rsa_pss_saltlen(pkctx, &saltlen))
        goto err;
    if (saltlen == -1)
        saltlen = EVP_MD_size(sigmd);
    else if (saltlen == -2) {
        saltlen = EVP_PKEY_size(pk) - EVP_MD_size(sigmd) - 2;
        if (((EVP_PKEY_bits(pk) - 1) & 0x7) == 0)
            saltlen--;
    }
    pss = RSA_PSS_PARAMS_new();
    if (!pss)
        goto err;
    if (saltlen != 20) {
        pss->saltLength = ASN1_INTEGER_new();
        if (!pss->saltLength)
            goto err;
        if (!ASN1_INTEGER_set(pss->saltLength, saltlen))
            goto err;
    }
    if (!rsa_md_to_algor(&pss->hashAlgorithm, sigmd))
        goto err;
    if (!rsa_md_to_mgf1(&pss->maskGenAlgorithm, mgf1md))
        goto err;
    /* Finally create string with pss parameter encoding. */
    if (!ASN1_item_pack(pss, ASN1_ITEM_rptr(RSA_PSS_PARAMS), &os))
         goto err;
    rv = 1;
 err:
    if (pss)
        RSA_PSS_PARAMS_free(pss);
    if (rv)
        return os;
    if (os)
        ASN1_STRING_free(os);
    return NULL;
}
