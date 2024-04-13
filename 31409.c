int tls1_set_curves(unsigned char **pext, size_t *pextlen,
                    int *curves, size_t ncurves)
{
    unsigned char *clist, *p;
    size_t i;
    /*
     * Bitmap of curves included to detect duplicates: only works while curve
     * ids < 32
     */
    unsigned long dup_list = 0;
# ifdef OPENSSL_NO_EC2M
    EC_GROUP *curve;
# endif

    clist = OPENSSL_malloc(ncurves * 2);
    if (!clist)
        return 0;
    for (i = 0, p = clist; i < ncurves; i++) {
        unsigned long idmask;
        int id;
        id = tls1_ec_nid2curve_id(curves[i]);
# ifdef OPENSSL_FIPS
        /* NB: 25 is last curve ID supported by FIPS module */
        if (FIPS_mode() && id > 25) {
            OPENSSL_free(clist);
            return 0;
        }
# endif
# ifdef OPENSSL_NO_EC2M
        curve = EC_GROUP_new_by_curve_name(curves[i]);
        if (!curve || EC_METHOD_get_field_type(EC_GROUP_method_of(curve))
            == NID_X9_62_characteristic_two_field) {
            if (curve)
                EC_GROUP_free(curve);
            OPENSSL_free(clist);
            return 0;
        } else
            EC_GROUP_free(curve);
# endif
        idmask = 1L << id;
        if (!id || (dup_list & idmask)) {
            OPENSSL_free(clist);
            return 0;
        }
        dup_list |= idmask;
        s2n(id, p);
    }
    if (*pext)
        OPENSSL_free(*pext);
    *pext = clist;
    *pextlen = ncurves * 2;
    return 1;
}
