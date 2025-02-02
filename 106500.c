GC_INNER void * GC_generic_malloc_inner(size_t lb, int k)
{
    void *op;

    if(SMALL_OBJ(lb)) {
        struct obj_kind * kind = GC_obj_kinds + k;
        size_t lg = GC_size_map[lb];
        void ** opp = &(kind -> ok_freelist[lg]);

        op = *opp;
        if (EXPECT(0 == op, FALSE)) {
            if (GC_size_map[lb] == 0) {
              if (!EXPECT(GC_is_initialized, TRUE)) GC_init();
              if (GC_size_map[lb] == 0) GC_extend_size_map(lb);
              return(GC_generic_malloc_inner(lb, k));
            }
            if (kind -> ok_reclaim_list == 0) {
                if (!GC_alloc_reclaim_list(kind)) goto out;
            }
            op = GC_allocobj(lg, k);
            if (op == 0) goto out;
        }
        *opp = obj_link(op);
        obj_link(op) = 0;
        GC_bytes_allocd += GRANULES_TO_BYTES(lg);
    } else {
        op = (ptr_t)GC_alloc_large_and_clear(ADD_SLOP(lb), k, 0);
        GC_bytes_allocd += lb;
    }

out:
    return op;
}
