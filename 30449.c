static int rsa_cms_encrypt(CMS_RecipientInfo *ri)
{
    const EVP_MD *md, *mgf1md;
    RSA_OAEP_PARAMS *oaep = NULL;
    ASN1_STRING *os = NULL;
    X509_ALGOR *alg;
    EVP_PKEY_CTX *pkctx = CMS_RecipientInfo_get0_pkey_ctx(ri);
    int pad_mode = RSA_PKCS1_PADDING, rv = 0, labellen;
    unsigned char *label;
    CMS_RecipientInfo_ktri_get0_algs(ri, NULL, NULL, &alg);
    if (pkctx) {
        if (EVP_PKEY_CTX_get_rsa_padding(pkctx, &pad_mode) <= 0)
            return 0;
    }
    if (pad_mode == RSA_PKCS1_PADDING) {
        X509_ALGOR_set0(alg, OBJ_nid2obj(NID_rsaEncryption), V_ASN1_NULL, 0);
        return 1;
    }
    /* Not supported */
    if (pad_mode != RSA_PKCS1_OAEP_PADDING)
        return 0;
    if (EVP_PKEY_CTX_get_rsa_oaep_md(pkctx, &md) <= 0)
        goto err;
    if (EVP_PKEY_CTX_get_rsa_mgf1_md(pkctx, &mgf1md) <= 0)
        goto err;
    labellen = EVP_PKEY_CTX_get0_rsa_oaep_label(pkctx, &label);
    if (labellen < 0)
        goto err;
    oaep = RSA_OAEP_PARAMS_new();
    if (!oaep)
        goto err;
    if (!rsa_md_to_algor(&oaep->hashFunc, md))
        goto err;
    if (!rsa_md_to_mgf1(&oaep->maskGenFunc, mgf1md))
        goto err;
    if (labellen > 0) {
        ASN1_OCTET_STRING *los = ASN1_OCTET_STRING_new();
        oaep->pSourceFunc = X509_ALGOR_new();
        if (!oaep->pSourceFunc)
            goto err;
        if (!los)
            goto err;
        if (!ASN1_OCTET_STRING_set(los, label, labellen)) {
            ASN1_OCTET_STRING_free(los);
            goto err;
        }
        X509_ALGOR_set0(oaep->pSourceFunc, OBJ_nid2obj(NID_pSpecified),
                        V_ASN1_OCTET_STRING, los);
    }
    /* create string with pss parameter encoding. */
    if (!ASN1_item_pack(oaep, ASN1_ITEM_rptr(RSA_OAEP_PARAMS), &os))
         goto err;
    X509_ALGOR_set0(alg, OBJ_nid2obj(NID_rsaesOaep), V_ASN1_SEQUENCE, os);
    os = NULL;
    rv = 1;
 err:
    if (oaep)
        RSA_OAEP_PARAMS_free(oaep);
    if (os)
        ASN1_STRING_free(os);
    return rv;
}
