static int ec_asn1_group2curve(const EC_GROUP *group, X9_62_CURVE *curve)
{
    int ok = 0, nid;
    BIGNUM *tmp_1 = NULL, *tmp_2 = NULL;
    unsigned char *buffer_1 = NULL, *buffer_2 = NULL,
        *a_buf = NULL, *b_buf = NULL;
    size_t len_1, len_2;
    unsigned char char_zero = 0;

    if (!group || !curve || !curve->a || !curve->b)
        return 0;

    if ((tmp_1 = BN_new()) == NULL || (tmp_2 = BN_new()) == NULL) {
        ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    nid = EC_METHOD_get_field_type(EC_GROUP_method_of(group));

    /* get a and b */
    if (nid == NID_X9_62_prime_field) {
        if (!EC_GROUP_get_curve_GFp(group, NULL, tmp_1, tmp_2, NULL)) {
            ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_EC_LIB);
            goto err;
        }
    } else {                    /* nid == NID_X9_62_characteristic_two_field */

        if (!EC_GROUP_get_curve_GF2m(group, NULL, tmp_1, tmp_2, NULL)) {
            ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_EC_LIB);
            goto err;
        }
    }

    len_1 = (size_t)BN_num_bytes(tmp_1);
    len_2 = (size_t)BN_num_bytes(tmp_2);

    if (len_1 == 0) {
        /* len_1 == 0 => a == 0 */
        a_buf = &char_zero;
        len_1 = 1;
    } else {
        if ((buffer_1 = OPENSSL_malloc(len_1)) == NULL) {
            ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_MALLOC_FAILURE);
            goto err;
        }
        if ((len_1 = BN_bn2bin(tmp_1, buffer_1)) == 0) {
            ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_BN_LIB);
            goto err;
        }
        a_buf = buffer_1;
    }

    if (len_2 == 0) {
        /* len_2 == 0 => b == 0 */
        b_buf = &char_zero;
        len_2 = 1;
    } else {
        if ((buffer_2 = OPENSSL_malloc(len_2)) == NULL) {
            ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_MALLOC_FAILURE);
            goto err;
        }
        if ((len_2 = BN_bn2bin(tmp_2, buffer_2)) == 0) {
            ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_BN_LIB);
            goto err;
        }
        b_buf = buffer_2;
    }

    /* set a and b */
    if (!M_ASN1_OCTET_STRING_set(curve->a, a_buf, len_1) ||
        !M_ASN1_OCTET_STRING_set(curve->b, b_buf, len_2)) {
        ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_ASN1_LIB);
        goto err;
    }

    /* set the seed (optional) */
    if (group->seed) {
        if (!curve->seed)
            if ((curve->seed = ASN1_BIT_STRING_new()) == NULL) {
                ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_MALLOC_FAILURE);
                goto err;
            }
        curve->seed->flags &= ~(ASN1_STRING_FLAG_BITS_LEFT | 0x07);
        curve->seed->flags |= ASN1_STRING_FLAG_BITS_LEFT;
        if (!ASN1_BIT_STRING_set(curve->seed, group->seed,
                                 (int)group->seed_len)) {
            ECerr(EC_F_EC_ASN1_GROUP2CURVE, ERR_R_ASN1_LIB);
            goto err;
        }
    } else {
        if (curve->seed) {
            ASN1_BIT_STRING_free(curve->seed);
            curve->seed = NULL;
        }
    }

    ok = 1;

 err:if (buffer_1)
        OPENSSL_free(buffer_1);
    if (buffer_2)
        OPENSSL_free(buffer_2);
    if (tmp_1)
        BN_free(tmp_1);
    if (tmp_2)
        BN_free(tmp_2);
    return (ok);
}
