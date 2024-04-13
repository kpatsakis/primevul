static CMS_EnvelopedData *cms_enveloped_data_init(CMS_ContentInfo *cms)
{
    if (cms->d.other == NULL) {
        cms->d.envelopedData = M_ASN1_new_of(CMS_EnvelopedData);
        if (!cms->d.envelopedData) {
            CMSerr(CMS_F_CMS_ENVELOPED_DATA_INIT, ERR_R_MALLOC_FAILURE);
            return NULL;
        }
        cms->d.envelopedData->version = 0;
        cms->d.envelopedData->encryptedContentInfo->contentType =
            OBJ_nid2obj(NID_pkcs7_data);
        ASN1_OBJECT_free(cms->contentType);
        cms->contentType = OBJ_nid2obj(NID_pkcs7_enveloped);
        return cms->d.envelopedData;
    }
    return cms_get0_enveloped(cms);
}
