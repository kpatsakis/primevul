find_realm_data(char *rname, krb5_ui_4 rsize)
{
    int i;
    for (i=0; i<kdc_numrealms; i++) {
        if ((rsize == strlen(kdc_realmlist[i]->realm_name)) &&
            !strncmp(rname, kdc_realmlist[i]->realm_name, rsize))
            return(kdc_realmlist[i]);
    }
    return((kdc_realm_t *) NULL);
}
