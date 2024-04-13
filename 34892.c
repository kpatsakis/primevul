jbig2_free(Jbig2Allocator *allocator, void *p)
{
    allocator->free(allocator, p);
}
