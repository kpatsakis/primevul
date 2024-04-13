GC_API void * GC_CALL GC_malloc_atomic_ignore_off_page(size_t lb)
{
    return((void *)GC_generic_malloc_ignore_off_page(lb, PTRFREE));
}
