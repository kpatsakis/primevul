int tls1_ec_curve_id2nid(int curve_id, unsigned int *pflags)
{
    const tls_curve_info *cinfo;
    /* ECC curves from RFC 4492 and RFC 7027 */
    if ((curve_id < 1) || ((unsigned int)curve_id > OSSL_NELEM(nid_list)))
        return 0;
    cinfo = nid_list + curve_id - 1;
    if (pflags)
        *pflags = cinfo->flags;
    return cinfo->nid;
}
