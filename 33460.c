EVP_PKEY_CTX *CMS_RecipientInfo_get0_pkey_ctx(CMS_RecipientInfo *ri)
{
    if (ri->type == CMS_RECIPINFO_TRANS)
        return ri->d.ktri->pctx;
    else if (ri->type == CMS_RECIPINFO_AGREE)
        return ri->d.kari->pctx;
    return NULL;
}
