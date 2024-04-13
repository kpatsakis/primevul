XNFcalloc(unsigned long amount)
{
    void *ret = calloc(1, amount);
    if (!ret)
        FatalError("XNFcalloc: Out of memory");
    return ret;
}
