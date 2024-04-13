static int cmykrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i;

    for (i = 0;i < 8;i+=2) {
        ptr[i] = 0;
        ptr[i+1] = 1;
    }
    return 0;
}
