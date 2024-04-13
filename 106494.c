STATIC ptr_t GC_alloc_large_and_clear(size_t lb, int k, unsigned flags)
{
    ptr_t result = GC_alloc_large(lb, k, flags);
    word n_blocks = OBJ_SZ_TO_BLOCKS(lb);

    if (0 == result) return 0;
    if (GC_debugging_started || GC_obj_kinds[k].ok_init) {
        /* Clear the whole block, in case of GC_realloc call. */
        BZERO(result, n_blocks * HBLKSIZE);
    }
    return result;
}
