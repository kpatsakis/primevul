CMS_ContentInfo *CMS_EnvelopedData_create(const EVP_CIPHER *cipher)
{
    CMS_ContentInfo *cms;
    CMS_EnvelopedData *env;
    cms = CMS_ContentInfo_new();
    if (cms == NULL)
        goto merr;
    env = cms_enveloped_data_init(cms);
    if (env == NULL)
        goto merr;
    if (!cms_EncryptedContent_init(env->encryptedContentInfo,
                                   cipher, NULL, 0))
        goto merr;
    return cms;
 merr:
    CMS_ContentInfo_free(cms);
    CMSerr(CMS_F_CMS_ENVELOPEDDATA_CREATE, ERR_R_MALLOC_FAILURE);
    return NULL;
}
