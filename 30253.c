static int hashdictkey(i_ctx_t * i_ctx_p, ref *CIEdict1, char *key, gs_md5_state_t *md5)
{
    int code;
    ref *tempref1;

    code = dict_find_string(CIEdict1, key, &tempref1);
    if (code <= 0)
        return 1;

    if (r_type(tempref1) == t_null)
        return 1;

    return hasharray(i_ctx_p, tempref1, md5);
}
