jbig2_alloc(Jbig2Allocator *allocator, size_t size, size_t num)
{
    /* check for integer multiplication overflow */
    if (num > 0 && size >= (size_t) - 0x100 / num)
        return NULL;
    return allocator->alloc(allocator, size * num);
}
