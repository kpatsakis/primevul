EC_GROUP *ec_asn1_pkparameters2group(const ECPKPARAMETERS *params)
{
    EC_GROUP *ret = NULL;
    int tmp = 0;

    if (params == NULL) {
        ECerr(EC_F_EC_ASN1_PKPARAMETERS2GROUP, EC_R_MISSING_PARAMETERS);
        return NULL;
    }

    if (params->type == 0) {    /* the curve is given by an OID */
        tmp = OBJ_obj2nid(params->value.named_curve);
        if ((ret = EC_GROUP_new_by_curve_name(tmp)) == NULL) {
            ECerr(EC_F_EC_ASN1_PKPARAMETERS2GROUP,
                  EC_R_EC_GROUP_NEW_BY_NAME_FAILURE);
            return NULL;
        }
        EC_GROUP_set_asn1_flag(ret, OPENSSL_EC_NAMED_CURVE);
    } else if (params->type == 1) { /* the parameters are given by a
                                     * ECPARAMETERS structure */
        ret = ec_asn1_parameters2group(params->value.parameters);
        if (!ret) {
            ECerr(EC_F_EC_ASN1_PKPARAMETERS2GROUP, ERR_R_EC_LIB);
            return NULL;
        }
        EC_GROUP_set_asn1_flag(ret, 0x0);
    } else if (params->type == 2) { /* implicitlyCA */
        return NULL;
    } else {
        ECerr(EC_F_EC_ASN1_PKPARAMETERS2GROUP, EC_R_ASN1_ERROR);
        return NULL;
    }

    return ret;
}
