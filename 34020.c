gs_create_pdf14trans(
    gs_composite_t **               ppct,
    const gs_pdf14trans_params_t *  pparams,
    gs_memory_t *                   mem )
{
    gs_pdf14trans_t *                pct;

    pct = gs_alloc_struct(mem, gs_pdf14trans_t, &st_pdf14trans,
                             "gs_create_pdf14trans");
    if (pct == NULL)
        return_error(gs_error_VMerror);
    pct->type = &gs_composite_pdf14trans_type;
    pct->id = gs_next_ids(mem, 1);
    pct->params = *pparams;
    pct->idle = false;
    *ppct = (gs_composite_t *)pct;
    return 0;
}
