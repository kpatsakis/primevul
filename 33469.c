CMS_RecipientInfo *CMS_add1_recipient_cert(CMS_ContentInfo *cms,
                                           X509 *recip, unsigned int flags)
{
    CMS_RecipientInfo *ri = NULL;
    CMS_EnvelopedData *env;
    EVP_PKEY *pk = NULL;
    env = cms_get0_enveloped(cms);
    if (!env)
        goto err;

    /* Initialize recipient info */
    ri = M_ASN1_new_of(CMS_RecipientInfo);
    if (!ri)
        goto merr;

    pk = X509_get0_pubkey(recip);
    if (!pk) {
        CMSerr(CMS_F_CMS_ADD1_RECIPIENT_CERT, CMS_R_ERROR_GETTING_PUBLIC_KEY);
        goto err;
    }

    switch (cms_pkey_get_ri_type(pk)) {

    case CMS_RECIPINFO_TRANS:
        if (!cms_RecipientInfo_ktri_init(ri, recip, pk, flags))
            goto err;
        break;

    case CMS_RECIPINFO_AGREE:
        if (!cms_RecipientInfo_kari_init(ri, recip, pk, flags))
            goto err;
        break;

    default:
        CMSerr(CMS_F_CMS_ADD1_RECIPIENT_CERT,
               CMS_R_NOT_SUPPORTED_FOR_THIS_KEY_TYPE);
        goto err;

    }

    if (!sk_CMS_RecipientInfo_push(env->recipientInfos, ri))
        goto merr;

    return ri;

 merr:
    CMSerr(CMS_F_CMS_ADD1_RECIPIENT_CERT, ERR_R_MALLOC_FAILURE);
 err:
    M_ASN1_free_of(ri, CMS_RecipientInfo);
    return NULL;

}
