static SRP_user_pwd *SRP_user_pwd_new()
{
    SRP_user_pwd *ret = OPENSSL_malloc(sizeof(SRP_user_pwd));
    if (ret == NULL)
        return NULL;
    ret->N = NULL;
    ret->g = NULL;
    ret->s = NULL;
    ret->v = NULL;
    ret->id = NULL;
    ret->info = NULL;
    return ret;
}
