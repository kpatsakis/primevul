GC_API void GC_CALL GC_incr_bytes_allocd(size_t n)
{
    GC_bytes_allocd += n;
}
