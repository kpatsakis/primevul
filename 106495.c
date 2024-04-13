STATIC GC_bool GC_alloc_reclaim_list(struct obj_kind *kind)
{
    struct hblk ** result = (struct hblk **)
                GC_scratch_alloc((MAXOBJGRANULES+1) * sizeof(struct hblk *));
    if (result == 0) return(FALSE);
    BZERO(result, (MAXOBJGRANULES+1)*sizeof(struct hblk *));
    kind -> ok_reclaim_list = result;
    return(TRUE);
}
