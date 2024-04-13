gs_is_pdf14trans_compositor(const gs_composite_t * pct)
{
    return (pct->type == &gs_composite_pdf14trans_type
                || pct->type == &gs_composite_pdf14trans_no_clist_writer_type);
}
