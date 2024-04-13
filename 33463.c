int CMS_RecipientInfo_ktri_cert_cmp(CMS_RecipientInfo *ri, X509 *cert)
{
    if (ri->type != CMS_RECIPINFO_TRANS) {
        CMSerr(CMS_F_CMS_RECIPIENTINFO_KTRI_CERT_CMP,
               CMS_R_NOT_KEY_TRANSPORT);
        return -2;
    }
    return cms_SignerIdentifier_cert_cmp(ri->d.ktri->rid, cert);
}
