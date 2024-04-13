GC_API void * GC_CALL GC_malloc_many(size_t lb)
{
    void *result;
    GC_generic_malloc_many((lb + EXTRA_BYTES + GRANULE_BYTES-1)
                           & ~(GRANULE_BYTES-1),
                           NORMAL, &result);
    return result;
}
