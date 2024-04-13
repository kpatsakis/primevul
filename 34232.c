SRP_user_pwd *SRP_VBASE_get_by_user(SRP_VBASE *vb, char *username)
{
    int i;
    SRP_user_pwd *user;
    unsigned char digv[SHA_DIGEST_LENGTH];
    unsigned char digs[SHA_DIGEST_LENGTH];
    EVP_MD_CTX ctxt;

    if (vb == NULL)
        return NULL;
    for (i = 0; i < sk_SRP_user_pwd_num(vb->users_pwd); i++) {
        user = sk_SRP_user_pwd_value(vb->users_pwd, i);
        if (strcmp(user->id, username) == 0)
            return user;
    }
 
 }
