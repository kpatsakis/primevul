static inline void drawInnerPath(SkCanvas* canvas, const SkPath& path, SkPaint& paint, int width)
{
#if PLATFORM(CHROMIUM) && OS(DARWIN)
    paint.setAlpha(128);
    paint.setStrokeWidth(width * 0.5f);
    canvas->drawPath(path, paint);
#endif
}
