static int hashciedefspace(i_ctx_t *i_ctx_p, ref *space, gs_md5_state_t *md5)
{
    int code = 0;
    ref CIEdict1, spacename;

    code = array_get(imemory, space, 0, &spacename);
    if (code < 0)
        return 0;
    gs_md5_append(md5, (const gs_md5_byte_t *)&spacename.value.pname, sizeof(spacename.value.pname));

    code = array_get(imemory, space, 1, &CIEdict1);
    if (code < 0)
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"WhitePoint", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"BlackPoint", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"RangeABC", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"DecodeABC", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"MatrixABC", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"RangeLMN", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"DecodeLMN", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"MatrixMN", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"RangeDEF", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"DecodeDEF", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"RangeHIJ", md5))
        return 0;
    if (!hashdictkey(i_ctx_p, &CIEdict1, (char *)"Table", md5))
        return 0;
    return 1;
}
