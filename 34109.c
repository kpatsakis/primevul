XNFalloc(unsigned long amount)
{
    void *ptr = malloc(amount);
    if (!ptr)
        FatalError("Out of memory");
    return ptr;
}
