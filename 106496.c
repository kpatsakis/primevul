  GC_INNER void * GC_core_malloc(size_t lb)
#else
  GC_API void * GC_CALL GC_malloc(size_t lb)
#endif
{
    void *op;
    void **opp;
    size_t lg;
    DCL_LOCK_STATE;

    if(SMALL_OBJ(lb)) {
        lg = GC_size_map[lb];
        opp = (void **)&(GC_objfreelist[lg]);
        LOCK();
        if (EXPECT((op = *opp) == 0, FALSE)) {
            UNLOCK();
            return (GENERAL_MALLOC((word)lb, NORMAL));
        }
        GC_ASSERT(0 == obj_link(op)
                  || ((word)obj_link(op)
                        <= (word)GC_greatest_plausible_heap_addr
                     && (word)obj_link(op)
                        >= (word)GC_least_plausible_heap_addr));
        *opp = obj_link(op);
        obj_link(op) = 0;
        GC_bytes_allocd += GRANULES_TO_BYTES(lg);
        UNLOCK();
        return op;
   } else {
       return(GENERAL_MALLOC(lb, NORMAL));
   }
}
