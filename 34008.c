c_pdf14trans_create_default_compositor(const gs_composite_t * pct,
    gx_device ** pp14dev, gx_device * tdev, gs_gstate * pgs,
    gs_memory_t * mem)
{
    const gs_pdf14trans_t * pdf14pct = (const gs_pdf14trans_t *) pct;
    gx_device * p14dev = NULL;
    int code = 0;

    /*
     * We only handle the push operation.  All other operations are ignored.
     * The other operations will be handled by the create_compositor routine
     * for the PDF 1.4 compositing device.
     */
    switch (pdf14pct->params.pdf14_op) {
        case PDF14_PUSH_DEVICE:
            code = gs_pdf14_device_push(mem, pgs, &p14dev, tdev, pdf14pct);
            *pp14dev = p14dev;
            break;
        default:
            *pp14dev = tdev;
            break;
    }
    return code;
}
