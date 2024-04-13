static int devicepdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int code;
    ref tref;

    code = array_get(imemory, space, 1, &tref);
    if (code < 0)
        return code;
    ptr[0] = 0;
    ptr[1] = (float)(1 << tref.value.intval);
    return 0;
}
