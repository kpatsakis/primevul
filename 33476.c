int cms_env_asn1_ctrl(CMS_RecipientInfo *ri, int cmd)
{
    EVP_PKEY *pkey;
    int i;
    if (ri->type == CMS_RECIPINFO_TRANS)
        pkey = ri->d.ktri->pkey;
    else if (ri->type == CMS_RECIPINFO_AGREE) {
        EVP_PKEY_CTX *pctx = ri->d.kari->pctx;
        if (!pctx)
            return 0;
        pkey = EVP_PKEY_CTX_get0_pkey(pctx);
        if (!pkey)
            return 0;
    } else
        return 0;
    if (!pkey->ameth || !pkey->ameth->pkey_ctrl)
        return 1;
    i = pkey->ameth->pkey_ctrl(pkey, ASN1_PKEY_CTRL_CMS_ENVELOPE, cmd, ri);
    if (i == -2) {
        CMSerr(CMS_F_CMS_ENV_ASN1_CTRL,
               CMS_R_NOT_SUPPORTED_FOR_THIS_KEY_TYPE);
        return 0;
    }
    if (i <= 0) {
        CMSerr(CMS_F_CMS_ENV_ASN1_CTRL, CMS_R_CTRL_FAILURE);
        return 0;
    }
    return 1;
}
