GC_INNER ptr_t GC_alloc_large(size_t lb, int k, unsigned flags)
{
    struct hblk * h;
    word n_blocks;
    ptr_t result;
    GC_bool retry = FALSE;

    /* Round up to a multiple of a granule. */
      lb = (lb + GRANULE_BYTES - 1) & ~(GRANULE_BYTES - 1);
    n_blocks = OBJ_SZ_TO_BLOCKS(lb);
    if (!EXPECT(GC_is_initialized, TRUE)) GC_init();
    /* Do our share of marking work */
        if (GC_incremental && !GC_dont_gc)
            GC_collect_a_little_inner((int)n_blocks);
    h = GC_allochblk(lb, k, flags);
#   ifdef USE_MUNMAP
        if (0 == h) {
            GC_merge_unmapped();
            h = GC_allochblk(lb, k, flags);
        }
#   endif
    while (0 == h && GC_collect_or_expand(n_blocks, flags != 0, retry)) {
        h = GC_allochblk(lb, k, flags);
        retry = TRUE;
    }
    if (h == 0) {
        result = 0;
    } else {
        size_t total_bytes = n_blocks * HBLKSIZE;
        if (n_blocks > 1) {
            GC_large_allocd_bytes += total_bytes;
            if (GC_large_allocd_bytes > GC_max_large_allocd_bytes)
                GC_max_large_allocd_bytes = GC_large_allocd_bytes;
        }
        result = h -> hb_body;
    }
    return result;
}
