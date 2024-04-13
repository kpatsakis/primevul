BIO *cms_EnvelopedData_init_bio(CMS_ContentInfo *cms)
{
    CMS_EncryptedContentInfo *ec;
    STACK_OF(CMS_RecipientInfo) *rinfos;
    CMS_RecipientInfo *ri;
    int i, ok = 0;
    BIO *ret;

    /* Get BIO first to set up key */

    ec = cms->d.envelopedData->encryptedContentInfo;
    ret = cms_EncryptedContent_init_bio(ec);

    /* If error or no cipher end of processing */

    if (!ret || !ec->cipher)
        return ret;

    /* Now encrypt content key according to each RecipientInfo type */

    rinfos = cms->d.envelopedData->recipientInfos;

    for (i = 0; i < sk_CMS_RecipientInfo_num(rinfos); i++) {
        ri = sk_CMS_RecipientInfo_value(rinfos, i);
        if (CMS_RecipientInfo_encrypt(cms, ri) <= 0) {
            CMSerr(CMS_F_CMS_ENVELOPEDDATA_INIT_BIO,
                   CMS_R_ERROR_SETTING_RECIPIENTINFO);
            goto err;
        }
    }
    cms_env_set_version(cms->d.envelopedData);

    ok = 1;

 err:
    ec->cipher = NULL;
    OPENSSL_clear_free(ec->key, ec->keylen);
    ec->key = NULL;
    ec->keylen = 0;
    if (ok)
        return ret;
    BIO_free(ret);
    return NULL;

}
