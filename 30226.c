static int devicenalternatespace(i_ctx_t * i_ctx_p, ref *space, ref **r, int *CIESubst)
{
    ref altspace;
    int code;

    code = array_get(imemory, space, 2, &altspace);
    if (code < 0)
        return code;
    ref_assign(*r, &altspace);
    return 0;
}
