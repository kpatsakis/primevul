int tls1_ec_curve_id2nid(int curve_id)
{
    /* ECC curves from RFC 4492 and RFC 7027 */
    if ((curve_id < 1) || ((unsigned int)curve_id >
                           sizeof(nid_list) / sizeof(nid_list[0])))
        return 0;
    return nid_list[curve_id - 1];
}
