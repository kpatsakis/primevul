GC_INNER void * GC_generic_malloc_inner_ignore_off_page(size_t lb, int k)
{
    word lb_adjusted;
    void * op;

    if (lb <= HBLKSIZE)
        return(GC_generic_malloc_inner(lb, k));
    lb_adjusted = ADD_SLOP(lb);
    op = GC_alloc_large_and_clear(lb_adjusted, k, IGNORE_OFF_PAGE);
    GC_bytes_allocd += lb_adjusted;
    return op;
}
