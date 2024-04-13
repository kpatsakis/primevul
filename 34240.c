static int SRP_user_pwd_set_ids(SRP_user_pwd *vinfo, const char *id,
                                const char *info)
{
    if (id != NULL && NULL == (vinfo->id = BUF_strdup(id)))
        return 0;
    return (info == NULL || NULL != (vinfo->info = BUF_strdup(info)));
}
