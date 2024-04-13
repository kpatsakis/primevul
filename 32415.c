static int tls1_set_ec_id(unsigned char *curve_id, unsigned char *comp_id,
                          EC_KEY *ec)
{
    int id;
    const EC_GROUP *grp;
    if (!ec)
        return 0;
    /* Determine if it is a prime field */
    grp = EC_KEY_get0_group(ec);
    if (!grp)
        return 0;
    /* Determine curve ID */
    id = EC_GROUP_get_curve_name(grp);
    id = tls1_ec_nid2curve_id(id);
    /* If no id return error: we don't support arbitrary explicit curves */
    if (id == 0)
        return 0;
    curve_id[0] = 0;
    curve_id[1] = (unsigned char)id;
    if (comp_id) {
        if (EC_KEY_get0_public_key(ec) == NULL)
            return 0;
        if (EC_KEY_get_conv_form(ec) == POINT_CONVERSION_UNCOMPRESSED) {
            *comp_id = TLSEXT_ECPOINTFORMAT_uncompressed;
        } else {
            if ((nid_list[id - 1].flags & TLS_CURVE_TYPE) == TLS_CURVE_PRIME)
                *comp_id = TLSEXT_ECPOINTFORMAT_ansiX962_compressed_prime;
            else
                *comp_id = TLSEXT_ECPOINTFORMAT_ansiX962_compressed_char2;
        }
    }
    return 1;
}
