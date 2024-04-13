CMS_RecipientInfo *CMS_add0_recipient_key(CMS_ContentInfo *cms, int nid,
                                          unsigned char *key, size_t keylen,
                                          unsigned char *id, size_t idlen,
                                          ASN1_GENERALIZEDTIME *date,
                                          ASN1_OBJECT *otherTypeId,
                                          ASN1_TYPE *otherType)
{
    CMS_RecipientInfo *ri = NULL;
    CMS_EnvelopedData *env;
    CMS_KEKRecipientInfo *kekri;
    env = cms_get0_enveloped(cms);
    if (!env)
        goto err;

    if (nid == NID_undef) {
        switch (keylen) {
        case 16:
            nid = NID_id_aes128_wrap;
            break;

        case 24:
            nid = NID_id_aes192_wrap;
            break;

        case 32:
            nid = NID_id_aes256_wrap;
            break;

        default:
            CMSerr(CMS_F_CMS_ADD0_RECIPIENT_KEY, CMS_R_INVALID_KEY_LENGTH);
            goto err;
        }

    } else {

        size_t exp_keylen = aes_wrap_keylen(nid);

        if (!exp_keylen) {
            CMSerr(CMS_F_CMS_ADD0_RECIPIENT_KEY,
                   CMS_R_UNSUPPORTED_KEK_ALGORITHM);
            goto err;
        }

        if (keylen != exp_keylen) {
            CMSerr(CMS_F_CMS_ADD0_RECIPIENT_KEY, CMS_R_INVALID_KEY_LENGTH);
            goto err;
        }

    }

    /* Initialize recipient info */
    ri = M_ASN1_new_of(CMS_RecipientInfo);
    if (!ri)
        goto merr;

    ri->d.kekri = M_ASN1_new_of(CMS_KEKRecipientInfo);
    if (!ri->d.kekri)
        goto merr;
    ri->type = CMS_RECIPINFO_KEK;

    kekri = ri->d.kekri;

    if (otherTypeId) {
        kekri->kekid->other = M_ASN1_new_of(CMS_OtherKeyAttribute);
        if (kekri->kekid->other == NULL)
            goto merr;
    }

    if (!sk_CMS_RecipientInfo_push(env->recipientInfos, ri))
        goto merr;

    /* After this point no calls can fail */

    kekri->version = 4;

    kekri->key = key;
    kekri->keylen = keylen;

    ASN1_STRING_set0(kekri->kekid->keyIdentifier, id, idlen);

    kekri->kekid->date = date;

    if (kekri->kekid->other) {
        kekri->kekid->other->keyAttrId = otherTypeId;
        kekri->kekid->other->keyAttr = otherType;
    }

    X509_ALGOR_set0(kekri->keyEncryptionAlgorithm,
                    OBJ_nid2obj(nid), V_ASN1_UNDEF, NULL);

    return ri;

 merr:
    CMSerr(CMS_F_CMS_ADD0_RECIPIENT_KEY, ERR_R_MALLOC_FAILURE);
 err:
    M_ASN1_free_of(ri, CMS_RecipientInfo);
    return NULL;

}
