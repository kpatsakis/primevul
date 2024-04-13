pdf14_fill_rectangle(gx_device * dev,
                    int x, int y, int w, int h, gx_color_index color)
{
    pdf14_device *pdev = (pdf14_device *)dev;
    pdf14_buf *buf = pdev->ctx->stack;

    fit_fill_xywh(dev, x, y, w, h);
    if (w <= 0 || h <= 0)
        return 0;
    if (buf->knockout)
        return pdf14_mark_fill_rectangle_ko_simple(dev, x, y, w, h, color, NULL,
                                                   false);
    else
        return pdf14_mark_fill_rectangle(dev, x, y, w, h, color, NULL, false);
}
