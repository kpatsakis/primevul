EC_GROUP *d2i_ECPKParameters(EC_GROUP **a, const unsigned char **in, long len)
{
    EC_GROUP *group = NULL;
    ECPKPARAMETERS *params = NULL;

    if ((params = d2i_ECPKPARAMETERS(NULL, in, len)) == NULL) {
        ECerr(EC_F_D2I_ECPKPARAMETERS, EC_R_D2I_ECPKPARAMETERS_FAILURE);
        ECPKPARAMETERS_free(params);
        return NULL;
    }

    if ((group = ec_asn1_pkparameters2group(params)) == NULL) {
        ECerr(EC_F_D2I_ECPKPARAMETERS, EC_R_PKPARAMETERS2GROUP_FAILURE);
        return NULL;
    }

    if (a && *a)
        EC_GROUP_clear_free(*a);
    if (a)
        *a = group;

    ECPKPARAMETERS_free(params);
    return (group);
}
