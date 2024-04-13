int tls1_ec_nid2curve_id(int nid)
{
    size_t i;
    for (i = 0; i < OSSL_NELEM(nid_list); i++) {
        if (nid_list[i].nid == nid)
            return i + 1;
    }
    return 0;
}
