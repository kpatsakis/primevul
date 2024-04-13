int tls1_set_curves_list(unsigned char **pext, size_t *pextlen,
                         const char *str)
{
    nid_cb_st ncb;
    ncb.nidcnt = 0;
    if (!CONF_parse_list(str, ':', 1, nid_cb, &ncb))
        return 0;
    if (pext == NULL)
        return 1;
    return tls1_set_curves(pext, pextlen, ncb.nid_arr, ncb.nidcnt);
}
