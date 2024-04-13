pdf14_fill_rectangle_hl_color(gx_device *dev, const gs_fixed_rect *rect,
    const gs_gstate *pgs, const gx_drawing_color *pdcolor,
    const gx_clip_path *pcpath)
{
    pdf14_device *pdev = (pdf14_device *)dev;
    pdf14_buf *buf = pdev->ctx->stack;
    int x = fixed2int(rect->p.x);
    int y = fixed2int(rect->p.y);
    int w = fixed2int(rect->q.x) - x;
    int h = fixed2int(rect->q.y) -y;

    fit_fill_xywh(dev, x, y, w, h);
    if (w <= 0 || h <= 0)
        return 0;
    if (buf->knockout)
        return pdf14_mark_fill_rectangle_ko_simple(dev, x, y, w, h, 0, pdcolor,
                                                   true);
    else
        return pdf14_mark_fill_rectangle(dev, x, y, w, h, 0, pdcolor, true);
}
