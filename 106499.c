  GC_INNER void GC_free_inner(void * p)
  {
    struct hblk *h;
    hdr *hhdr;
    size_t sz; /* bytes */
    size_t ngranules;  /* sz in granules */
    void ** flh;
    int knd;
    struct obj_kind * ok;

    h = HBLKPTR(p);
    hhdr = HDR(h);
    knd = hhdr -> hb_obj_kind;
    sz = hhdr -> hb_sz;
    ngranules = BYTES_TO_GRANULES(sz);
    ok = &GC_obj_kinds[knd];
    if (ngranules <= MAXOBJGRANULES) {
        GC_bytes_freed += sz;
        if (IS_UNCOLLECTABLE(knd)) GC_non_gc_bytes -= sz;
        if (ok -> ok_init) {
            BZERO((word *)p + 1, sz-sizeof(word));
        }
        flh = &(ok -> ok_freelist[ngranules]);
        obj_link(p) = *flh;
        *flh = (ptr_t)p;
    } else {
        size_t nblocks = OBJ_SZ_TO_BLOCKS(sz);
        GC_bytes_freed += sz;
        if (IS_UNCOLLECTABLE(knd)) GC_non_gc_bytes -= sz;
        if (nblocks > 1) {
          GC_large_allocd_bytes -= nblocks * HBLKSIZE;
        }
        GC_freehblk(h);
    }
  }
