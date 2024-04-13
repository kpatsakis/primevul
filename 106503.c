GC_API void * GC_CALL GC_malloc_uncollectable(size_t lb)
{
    void *op;
    void **opp;
    size_t lg;
    DCL_LOCK_STATE;

    if( SMALL_OBJ(lb) ) {
        if (EXTRA_BYTES != 0 && lb != 0) lb--;
                  /* We don't need the extra byte, since this won't be  */
                  /* collected anyway.                                  */
        lg = GC_size_map[lb];
        opp = &(GC_uobjfreelist[lg]);
        LOCK();
        op = *opp;
        if (EXPECT(0 != op, TRUE)) {
            *opp = obj_link(op);
            obj_link(op) = 0;
            GC_bytes_allocd += GRANULES_TO_BYTES(lg);
            /* Mark bit ws already set on free list.  It will be        */
            /* cleared only temporarily during a collection, as a       */
            /* result of the normal free list mark bit clearing.        */
            GC_non_gc_bytes += GRANULES_TO_BYTES(lg);
            UNLOCK();
        } else {
            UNLOCK();
            op = (ptr_t)GC_generic_malloc((word)lb, UNCOLLECTABLE);
            /* For small objects, the free lists are completely marked. */
        }
        GC_ASSERT(0 == op || GC_is_marked(op));
        return((void *) op);
    } else {
        hdr * hhdr;

        op = (ptr_t)GC_generic_malloc((word)lb, UNCOLLECTABLE);
        if (0 == op) return(0);

        GC_ASSERT(((word)op & (HBLKSIZE - 1)) == 0); /* large block */
        hhdr = HDR(op);
        /* We don't need the lock here, since we have an undisguised    */
        /* pointer.  We do need to hold the lock while we adjust        */
        /* mark bits.                                                   */
        LOCK();
        set_mark_bit_from_hdr(hhdr, 0); /* Only object. */
        GC_ASSERT(hhdr -> hb_n_marks == 0);
        hhdr -> hb_n_marks = 1;
        UNLOCK();
        return((void *) op);
    }
}
