  GC_INNER void * GC_core_malloc_atomic(size_t lb)
#else
  GC_API void * GC_CALL GC_malloc_atomic(size_t lb)
#endif
{
    void *op;
    void ** opp;
    size_t lg;
    DCL_LOCK_STATE;

    if(SMALL_OBJ(lb)) {
        lg = GC_size_map[lb];
        opp = &(GC_aobjfreelist[lg]);
        LOCK();
        if (EXPECT((op = *opp) == 0, FALSE)) {
            UNLOCK();
            return(GENERAL_MALLOC((word)lb, PTRFREE));
        }
        *opp = obj_link(op);
        GC_bytes_allocd += GRANULES_TO_BYTES(lg);
        UNLOCK();
        return((void *) op);
   } else {
       return(GENERAL_MALLOC((word)lb, PTRFREE));
   }
}
