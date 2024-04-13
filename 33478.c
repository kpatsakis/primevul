static void cms_env_set_version(CMS_EnvelopedData *env)
{
    int i;
    CMS_RecipientInfo *ri;

    /*
     * Can't set version higher than 4 so if 4 or more already nothing to do.
     */
    if (env->version >= 4)
        return;

    cms_env_set_originfo_version(env);

    if (env->version >= 3)
        return;

    for (i = 0; i < sk_CMS_RecipientInfo_num(env->recipientInfos); i++) {
        ri = sk_CMS_RecipientInfo_value(env->recipientInfos, i);
        if (ri->type == CMS_RECIPINFO_PASS || ri->type == CMS_RECIPINFO_OTHER) {
            env->version = 3;
            return;
        } else if (ri->type != CMS_RECIPINFO_TRANS
                   || ri->d.ktri->version != 0) {
            env->version = 2;
        }
    }
    if (env->originatorInfo || env->unprotectedAttrs)
        env->version = 2;
    if (env->version == 2)
        return;
    env->version = 0;
}
