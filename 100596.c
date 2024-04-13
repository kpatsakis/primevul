void GraphicsContext::clipOut(const Path& p)
{
    if (paintingDisabled())
        return;

    SkPath path = *p.platformPath();
    if (!isPathSkiaSafe(getCTM(), path))
        return;

    path.toggleInverseFillType();
    platformContext()->clipPathAntiAliased(path);
}
