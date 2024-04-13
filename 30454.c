static int rsa_md_to_mgf1(X509_ALGOR **palg, const EVP_MD *mgf1md)
{
    X509_ALGOR *algtmp = NULL;
    ASN1_STRING *stmp = NULL;
    *palg = NULL;
    if (EVP_MD_type(mgf1md) == NID_sha1)
        return 1;
    /* need to embed algorithm ID inside another */
    if (!rsa_md_to_algor(&algtmp, mgf1md))
        goto err;
    if (!ASN1_item_pack(algtmp, ASN1_ITEM_rptr(X509_ALGOR), &stmp))
         goto err;
    *palg = X509_ALGOR_new();
    if (!*palg)
        goto err;
    X509_ALGOR_set0(*palg, OBJ_nid2obj(NID_mgf1), V_ASN1_SEQUENCE, stmp);
    stmp = NULL;
 err:
    if (stmp)
        ASN1_STRING_free(stmp);
    if (algtmp)
        X509_ALGOR_free(algtmp);
    if (*palg)
        return 1;
    return 0;
}
