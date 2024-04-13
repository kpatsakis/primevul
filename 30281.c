static int separationalternatespace(i_ctx_t * i_ctx_p, ref *sepspace, ref **r, int *CIESubst)
{
    ref tref;
    int code;

    code = array_get(imemory, sepspace, 2, &tref);
    if (code < 0)
        return code;
    ref_assign(*r, &tref);
    return 0;
}
