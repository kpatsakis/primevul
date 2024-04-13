static int cms_kari_set1_pkey(CMS_ContentInfo *cms, CMS_RecipientInfo *ri,
                              EVP_PKEY *pk, X509 *cert)
{
    int i;
    STACK_OF(CMS_RecipientEncryptedKey) *reks;
    CMS_RecipientEncryptedKey *rek;
    reks = CMS_RecipientInfo_kari_get0_reks(ri);
    for (i = 0; i < sk_CMS_RecipientEncryptedKey_num(reks); i++) {
        int rv;
        rek = sk_CMS_RecipientEncryptedKey_value(reks, i);
        if (cert != NULL && CMS_RecipientEncryptedKey_cert_cmp(rek, cert))
            continue;
        CMS_RecipientInfo_kari_set0_pkey(ri, pk);
        rv = CMS_RecipientInfo_kari_decrypt(cms, ri, rek);
        CMS_RecipientInfo_kari_set0_pkey(ri, NULL);
        if (rv > 0)
            return 1;
        return cert == NULL ? 0 : -1;
    }
    return 0;
}
