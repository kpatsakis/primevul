static ECPARAMETERS *ec_asn1_group2parameters(const EC_GROUP *group,
                                              ECPARAMETERS *param)
{
    int ok = 0;
    size_t len = 0;
    ECPARAMETERS *ret = NULL;
    BIGNUM *tmp = NULL;
    unsigned char *buffer = NULL;
    const EC_POINT *point = NULL;
    point_conversion_form_t form;

    if ((tmp = BN_new()) == NULL) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    if (param == NULL) {
        if ((ret = ECPARAMETERS_new()) == NULL) {
            ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_MALLOC_FAILURE);
            goto err;
        }
    } else
        ret = param;

    /* set the version (always one) */
    ret->version = (long)0x1;

    /* set the fieldID */
    if (!ec_asn1_group2fieldid(group, ret->fieldID)) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_EC_LIB);
        goto err;
    }

    /* set the curve */
    if (!ec_asn1_group2curve(group, ret->curve)) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_EC_LIB);
        goto err;
    }

    /* set the base point */
    if ((point = EC_GROUP_get0_generator(group)) == NULL) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, EC_R_UNDEFINED_GENERATOR);
        goto err;
    }

    form = EC_GROUP_get_point_conversion_form(group);

    len = EC_POINT_point2oct(group, point, form, NULL, len, NULL);
    if (len == 0) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_EC_LIB);
        goto err;
    }
    if ((buffer = OPENSSL_malloc(len)) == NULL) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_MALLOC_FAILURE);
        goto err;
    }
    if (!EC_POINT_point2oct(group, point, form, buffer, len, NULL)) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_EC_LIB);
        goto err;
    }
    if (ret->base == NULL && (ret->base = ASN1_OCTET_STRING_new()) == NULL) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_MALLOC_FAILURE);
        goto err;
    }
    if (!ASN1_OCTET_STRING_set(ret->base, buffer, len)) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_ASN1_LIB);
        goto err;
    }

    /* set the order */
    if (!EC_GROUP_get_order(group, tmp, NULL)) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_EC_LIB);
        goto err;
    }
    ret->order = BN_to_ASN1_INTEGER(tmp, ret->order);
    if (ret->order == NULL) {
        ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_ASN1_LIB);
        goto err;
    }

    /* set the cofactor (optional) */
    if (EC_GROUP_get_cofactor(group, tmp, NULL)) {
        ret->cofactor = BN_to_ASN1_INTEGER(tmp, ret->cofactor);
        if (ret->cofactor == NULL) {
            ECerr(EC_F_EC_ASN1_GROUP2PARAMETERS, ERR_R_ASN1_LIB);
            goto err;
        }
    }

    ok = 1;

 err:if (!ok) {
        if (ret && !param)
            ECPARAMETERS_free(ret);
        ret = NULL;
    }
    if (tmp)
        BN_free(tmp);
    if (buffer)
        OPENSSL_free(buffer);
    return (ret);
}
