static int tls1_set_ec_id(unsigned char *curve_id, unsigned char *comp_id,
                          EC_KEY *ec)
{
    int is_prime, id;
    const EC_GROUP *grp;
    const EC_METHOD *meth;
    if (!ec)
        return 0;
    /* Determine if it is a prime field */
    grp = EC_KEY_get0_group(ec);
    if (!grp)
        return 0;
    meth = EC_GROUP_method_of(grp);
    if (!meth)
        return 0;
    if (EC_METHOD_get_field_type(meth) == NID_X9_62_prime_field)
        is_prime = 1;
    else
        is_prime = 0;
    /* Determine curve ID */
    id = EC_GROUP_get_curve_name(grp);
    id = tls1_ec_nid2curve_id(id);
    /* If we have an ID set it, otherwise set arbitrary explicit curve */
    if (id) {
        curve_id[0] = 0;
        curve_id[1] = (unsigned char)id;
    } else {
        curve_id[0] = 0xff;
        if (is_prime)
            curve_id[1] = 0x01;
        else
            curve_id[1] = 0x02;
    }
    if (comp_id) {
        if (EC_KEY_get0_public_key(ec) == NULL)
            return 0;
        if (EC_KEY_get_conv_form(ec) == POINT_CONVERSION_COMPRESSED) {
            if (is_prime)
                *comp_id = TLSEXT_ECPOINTFORMAT_ansiX962_compressed_prime;
            else
                *comp_id = TLSEXT_ECPOINTFORMAT_ansiX962_compressed_char2;
        } else
            *comp_id = TLSEXT_ECPOINTFORMAT_uncompressed;
    }
    return 1;
}
