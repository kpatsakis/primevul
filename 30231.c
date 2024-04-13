static int devicendomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, limit, code;
    ref namesarray;

    code = array_get(imemory, space, 1, &namesarray);
    if (code < 0)
        return code;

    limit = r_size(&namesarray) * 2;
    for (i = 0;i < limit;i+=2) {
        ptr[i] = 0;
        ptr[i+1] = 1;
    }
    return 0;
}
