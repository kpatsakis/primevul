static int indexeddomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    ref hival;
    int code;

    code = array_get(imemory, space, 2, &hival);
    if (code < 0)
        return code;
    ptr[0] = 0;
    ptr[1] = (float)hival.value.intval;
    return 0;
}
