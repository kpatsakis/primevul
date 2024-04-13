static int indexedalternatespace(i_ctx_t * i_ctx_p, ref *space, ref **r, int *CIESubst)
{
    ref alt;
    int code;

    code = array_get(imemory, *r, 1, &alt);
    if (code < 0)
        return code;
    ref_assign(*r, &alt);
    return 0;
}
