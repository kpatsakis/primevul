void GraphicsContext::drawLineForText(const FloatPoint& pt,
                                      float width,
                                      bool printing)
{
    if (paintingDisabled())
        return;

    if (width <= 0)
        return;

    int thickness = SkMax32(static_cast<int>(strokeThickness()), 1);
    SkRect r;
    r.fLeft = WebCoreFloatToSkScalar(pt.x());
    r.fTop = WebCoreFloatToSkScalar(pt.y());
    r.fRight = r.fLeft + WebCoreFloatToSkScalar(width);
    r.fBottom = r.fTop + SkIntToScalar(thickness);

    SkPaint paint;
    platformContext()->setupPaintForFilling(&paint);
    paint.setColor(platformContext()->effectiveStrokeColor());
    platformContext()->canvas()->drawRect(r, paint);
}
