GC_API void GC_CALL GC_incr_bytes_freed(size_t n)
{
    GC_bytes_freed += n;
}
