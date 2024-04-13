static int cieacompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    int code = 0;
    ref CIEdict1, CIEdict2;

    code = array_get(imemory, space, 1, &CIEdict1);
    if (code < 0)
        return 0;
    code = array_get(imemory, testspace, 1, &CIEdict2);
    if (code < 0)
        return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"WhitePoint"))
        return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"BlackPoint"))
        return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeA"))
        return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeA"))
        return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixA"))
        return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"RangeLMN"))
        return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"DecodeLMN"))
        return 0;
    if (!comparedictkey(i_ctx_p, &CIEdict1, &CIEdict2, (char *)"MatrixMN"))
        return 0;
    return 1;
}
