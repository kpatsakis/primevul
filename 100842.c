RenderImageResource* ImageLoader::renderImageResource()
{
    RenderObject* renderer = m_element->renderer();

    if (!renderer)
        return 0;

    if (renderer->isImage() && !static_cast<RenderImage*>(renderer)->isGeneratedContent())
        return toRenderImage(renderer)->imageResource();

#if ENABLE(SVG)
    if (renderer->isSVGImage())
        return toRenderSVGImage(renderer)->imageResource();
#endif

    if (renderer->isVideo())
        return toRenderVideo(renderer)->imageResource();

    return 0;
}
