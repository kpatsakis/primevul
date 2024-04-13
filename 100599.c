void GraphicsContext::fillRect(const FloatRect& rect, const Color& color, ColorSpace colorSpace)
{
    if (paintingDisabled())
        return;

    SkRect r = rect;
    if (!isRectSkiaSafe(getCTM(), r)) {
        ClipRectToCanvas(*platformContext()->canvas(), r, &r);
    }

    SkPaint paint;
    platformContext()->setupPaintCommon(&paint);
    paint.setColor(color.rgb());
    platformContext()->canvas()->drawRect(r, paint);
}
