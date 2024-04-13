send_pdf14trans(gs_gstate	* pgs, gx_device * dev,
    gx_device * * pcdev, gs_pdf14trans_params_t * pparams, gs_memory_t * mem)
{
    gs_composite_t * pct = NULL;
    int code;

    pparams->ctm = ctm_only(pgs);
    code = gs_create_pdf14trans(&pct, pparams, mem);
    if (code < 0)
        return code;
    code = dev_proc(dev, create_compositor) (dev, pcdev, pct, pgs, mem, NULL);
    if (code == gs_error_handled)
        code = 0;

    gs_free_object(pgs->memory, pct, "send_pdf14trans");

    return code;
}
