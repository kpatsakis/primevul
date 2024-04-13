static int devicencomponents(i_ctx_t * i_ctx_p, ref *space, int *n)
{
    ref namesarray;
    int code;

    code = array_get(imemory, space, 1, &namesarray);
    if (code < 0)
        return code;
    *n = r_size(&namesarray);
    return 0;
}
