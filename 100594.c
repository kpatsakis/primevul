void GraphicsContext::clearPlatformShadow()
{
    if (paintingDisabled())
        return;
    platformContext()->setDrawLooper(0);
}
