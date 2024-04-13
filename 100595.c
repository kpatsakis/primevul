void GraphicsContext::clip(const FloatRect& rect)
{
    if (paintingDisabled())
        return;

    SkRect r(rect);
    if (!isRectSkiaSafe(getCTM(), r))
        return;

    platformContext()->canvas()->clipRect(r);
}
